#include "muehle_fsm.test.h"
#include "muehle/muehle_fsm.h"

// following code is taken from https://github.com/boost-ext/sml/blob/4da1ad8c91d5e50642e0271b657baeac1d708bd6/example/plant_uml.cpp

inline void do_indent(unsigned int indent) { std::cout << std::string(indent, ' '); }

bool state_initialized = false;  // NOLINT(misc-definitions-in-headers)
std::vector<std::string> completed_submachines;  // NOLINT(misc-definitions-in-headers)

/** allows for checking if the type is sml::front::seq_
 * This type is used by sml when there are lists of actions.
 */
template <class... Ts>
struct is_seq_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_seq_<sml::front::seq_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::not_
 * This type is used by sml inside of guards, when the guard value is negated with !
 *
 * The partial specialization matches if the type passed in is sml::front::not_, causing the struct to
 * inherit from sml::aux::true_type, which gives it a member called "value" that is set to true.
 * If the type passed doesn't match sml::front::not_, it'll match the generic is_not_ which inherits
 * from sml::aux::false_type, giving it a member called "value" that is set to false.
 */
template <class... Ts>
struct is_not_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_not_<sml::front::not_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** provides access to the template parameter type of an sml::front::not_<T>
 */
template <class T>
struct strip_not_ {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct strip_not_<sml::front::not_<T>> {
  using type = T;
};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::and_
 * This type is used by sml inside of guards when two guard functions are combined with &&
 */
template <class... Ts>
struct is_and_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_and_<sml::front::and_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::or_
 * This type is used by sml inside of guards when two guard functions are combined with ||
 */
template <class... Ts>
struct is_or_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_or_<sml::front::or_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** uses std::tuple_element and std::tuple to access the Nth type in a parameter pack
 */
template <int N, class... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

/** gets the size of a parameter pack
 * this isn't really necessary, sizeof...(Ts) can be used directly instead
 */
template <class... Ts>
struct count {                                     // NOLINT(readability-identifier-naming)
  static const std::size_t value = sizeof...(Ts);  // NOLINT(readability-identifier-naming)
};

/** allows for checking if the type is sml::aux::zero_wrapper
 * sml puts this around types inside of guards and event sequences
 */
template <class T>
struct is_zero_wrapper : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class T>
struct is_zero_wrapper<sml::aux::zero_wrapper<T>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** if T is a zero wrapper, ::type will be the inner type. if not, it will be T.
 */
template <class T>
struct strip_zero_wrapper {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct strip_zero_wrapper<sml::aux::zero_wrapper<T>> {
  using type = T;
};  // NOLINT(readability-identifier-naming)

/** accesses the type of a state-machine, sml::back::sm
 */
template <class T>
struct submachine_type {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct submachine_type<sml::back::sm<T>> {
  using type = typename T::sm;
};  // NOLINT(readability-identifier-naming)

/** print the types inside a sml::front::seq_
 * These types came from a list of actions.
 */
template <class... Ts>
struct print_seq_types {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func() {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
      using current_type = NthTypeOf<I, Ts...>;
      if constexpr (is_seq_<typename current_type::type>::value) {  // NOLINT(readability-braces-around-statements)
        print_seq_types<typename current_type::type>::template func<0>();
      } else {  // NOLINT(readability-misleading-indentation)
        std::cout << sml::aux::string<typename strip_zero_wrapper<current_type>::type>{}.c_str();
      }
      if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        std::cout << ",\\n ";
      }
      print_seq_types<Ts...>::template func<I + 1>();
    }
  }
};
template <class... Ts>
struct print_seq_types<sml::front::seq_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func() {
    print_seq_types<Ts...>::template func<0>();
  }
};

/** print the types inside a guard
 * These can be a functor, an sml::front::not_, an sml::front::and_, or an sml::front::or_ which makes
 * this one more complicated. They also involve the zero_wrapper.
 * The various partial specializations handle all of the possible types.
 */
template <class... Ts>
struct print_guard {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(const std::string& sep = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
      using current_type = NthTypeOf<I, Ts...>;
      if constexpr (is_zero_wrapper<
                        current_type>::value) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        print_guard<typename strip_zero_wrapper<current_type>::type>::template func<0>();
      } else {  // NOLINT(readability-misleading-indentation)
        std::cout << sml::aux::string<current_type>{}.c_str();
      }

      if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        if (!sep.empty()) {
          std::cout << sep;
        }
      }

      print_guard<Ts...>::template func<I + 1>(sep);
    }
  }
};
template <class T>
struct print_guard<sml::front::not_<T>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(const std::string& /*sep*/ = "") {
    std::cout << "!" << sml::aux::string<typename strip_zero_wrapper<T>::type>{}.c_str();
  }
};
template <class... Ts>
struct print_guard<sml::front::and_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(const std::string& /*sep*/ = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {
      print_guard<Ts...>::template func<I>(" &&\\n ");
    }
  }
};
template <class... Ts>
struct print_guard<sml::front::or_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(const std::string& /*sep*/ = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {
      print_guard<Ts...>::template func<I>(" ||\\n ");
    }
  }
};

template <typename...>
struct dump_transitions;

template <int N, class T>
void dump_transition() noexcept {
  constexpr auto src_is_sub_sm =
      !sml::aux::is_same<sml::aux::type_list<>, sml::back::get_sub_sms<typename T::src_state>>::value;
  constexpr auto dst_is_sub_sm =
      !sml::aux::is_same<sml::aux::type_list<>, sml::back::get_sub_sms<typename T::dst_state>>::value;

  std::string src_state, dst_state;
  if constexpr (src_is_sub_sm) {
    src_state = std::string{sml::aux::string<typename submachine_type<typename T::src_state>::type>{}.c_str()};
  } else {  // NOLINT(readability-misleading-indentation)
    src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
  }

  if constexpr (dst_is_sub_sm) {
    dst_state = std::string{sml::aux::string<typename submachine_type<typename T::dst_state>::type>{}.c_str()};
  } else {  // NOLINT(readability-misleading-indentation)
    dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};
  }

  const auto dst_internal = sml::aux::is_same<typename T::dst_state, sml::front::internal>::value;

  const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
  const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
  const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

  if (has_event && has_action && sml::aux::is_same<typename T::action::type, sml::front::actions::defer>::value) {
    do_indent(N);
    std::cout << src_state << " : " << boost::sml::aux::get_type_name<typename T::event>() << " / defer" << std::endl;
    return;
  }

  if (dst_state == "terminate") {
    dst_state = "[*]";
  }

  if (T::initial) {
    if (state_initialized) {  // create an orthogonal section
      do_indent(N);
      std::cout << "--" << std::endl;
    }

    state_initialized = true;
    do_indent(N);
    std::cout << "[*] --> " << src_state << std::endl;
  }

  if constexpr (src_is_sub_sm) {
    auto already_in =
        std::find(completed_submachines.begin(), completed_submachines.end(), src_state) != completed_submachines.end();
    if (!already_in) {
      completed_submachines.push_back(src_state);
      constexpr int indent = N + 2;
      do_indent(N);
      std::cout << "state " << src_state << " {" << std::endl;
      bool prev_state = state_initialized;
      state_initialized = false;
      dump_transitions<typename T::src_state::transitions>::template func<indent>();
      do_indent(N);
      std::cout << "}" << std::endl;
      state_initialized = prev_state;
    }
  }

  do_indent(N);
  std::cout << src_state;
  if (!dst_internal) {
    std::cout << " --> " << dst_state;
  }

  if (has_event || has_guard || has_action) {
    std::cout << " :";
  }

  if (has_event) {
    std::cout << " " << std::string{sml::aux::string<typename T::event>{}.c_str()};
  }

  if (has_guard) {
    std::cout << "\\n [";
    print_guard<typename T::guard::type>::template func<0>();
    std::cout << "]";
  }

  if (has_action) {
    std::cout << " /\\n ";

    if constexpr (is_seq_<typename T::action::type>::value) {  // NOLINT(readability-braces-around-statements)
      std::cout << "(";
      print_seq_types<typename T::action::type>::template func<0>();
      std::cout << ")";
    } else {  // NOLINT(readability-misleading-indentation)
      std::cout << sml::aux::string<typename T::action::type>{}.c_str();
    }
  }

  std::cout << std::endl;

  if constexpr (dst_is_sub_sm) {
    auto already_in =
        std::find(completed_submachines.begin(), completed_submachines.end(), dst_state) != completed_submachines.end();
    if (!already_in) {
      completed_submachines.push_back(dst_state);
      constexpr int indent = N + 2;
      do_indent(N);
      std::cout << "state " << dst_state << " {" << std::endl;
      bool prev_state = state_initialized;
      state_initialized = false;
      dump_transitions<typename T::dst_state::transitions>::template func<indent>();
      do_indent(N);
      std::cout << "}" << std::endl;
      state_initialized = prev_state;
    }
  }
}

template <int INDENT, int I, class... Ts>
void apply_dump_transition() {
  constexpr auto param_pack_empty = (sizeof...(Ts) == I);
  if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
    dump_transition<INDENT, NthTypeOf<I, Ts...>>();
    apply_dump_transition<INDENT, I + 1, Ts...>();
  }
}

template <typename...>
struct dump_transitions {  // NOLINT(readability-identifier-naming)
  template <int INDENT>
  static void func() {}
};

template <typename... Ts>
struct dump_transitions<typename sml::aux::type_list<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int INDENT>
  static void func() {
    apply_dump_transition<INDENT, 0, Ts...>();
  }
};

template <class T>
void dump() noexcept {
  std::cout << "@startuml" << std::endl << std::endl;
  dump_transitions<typename sml::sm<T>::transitions>::template func<0>();
  std::cout << std::endl << "@enduml" << std::endl;
}

Muehle_FSM_Test::Muehle_FSM_Test()
{
}

TEST_F(Muehle_FSM_Test, testNewGame)
{
    dump<muehle::Transition_Table>(); // remove "muehle::" from output and use with https://www.plantuml.com/plantuml/uml/
    muehle::Fsm fsm(&mAction_Handler_Mock);
    EXPECT_CALL(mAction_Handler_Mock, start_new_game());
    EXPECT_CALL(mAction_Handler_Mock, set_selectable_game_pieces());
    fsm.get_fsm().process_event(muehle::Trigger_Start_New_Game());
}
