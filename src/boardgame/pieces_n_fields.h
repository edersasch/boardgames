#ifndef SRC_BOARDGAME_PIECES_N_FIELDS
#define SRC_BOARDGAME_PIECES_N_FIELDS

#include <nlohmann/json.hpp>

#include <algorithm>
#include <vector>
#include <array>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace boardgame {

namespace detail {

template <typename T>
struct Element_Number { int v {-1}; }; // default constructed means unset
template <typename T>
constexpr bool operator==(const Element_Number<T>& lhs, const Element_Number<T>& rhs) { return lhs.v == rhs.v; }
template <typename T>
constexpr bool operator!=(const Element_Number<T>& lhs, const Element_Number<T>& rhs) { return lhs.v != rhs.v; }

template <typename T>
struct Element_Range { int v; };
template <typename T>
constexpr bool operator==(const Element_Range<T>& lhs, const Element_Range<T>& rhs) { return lhs.v == rhs.v; }

template <typename T>
constexpr Element_Number<T> operator+(Element_Number<T> lhs, const Element_Range<T> rhs)
{
    lhs.v += rhs.v;
    return lhs;
}

template <typename T>
constexpr Element_Range<T> operator+(Element_Range<T> lhs, const Element_Range<T> rhs)
{
    lhs.v += rhs.v;
    return lhs;
}

template <typename T, typename U>
struct Elementgroup
{
    const T origin;
    const T begin_of_group;
    const T end_of_group;
    const U tag;
};

template <typename T, typename U>
constexpr auto make_elementgroup(const T& elements, const Element_Number<U> start, const Element_Range<U> range)
{
    auto o = std::cbegin(elements); // origin
    auto b = o + start.v;           // begin of group
    auto e = b + range.v;           // end of group
    return Elementgroup<decltype(b), U>{o, b, e, {}};
}

template <typename T>
constexpr bool is_set(const T& elem)
{
    return elem != T{};
}

template <typename T>
constexpr bool is_unset(const T& elem)
{
    return elem == T{};
}

template <typename T>
constexpr auto first_set_element(const T& elemgr)
{
    auto found = std::find_if_not(elemgr.begin_of_group, elemgr.end_of_group, &is_unset<decltype(*elemgr.begin_of_group)>);
    return found == elemgr.end_of_group ? decltype(*elemgr.begin_of_group){} : *found;
}

template <typename T>
constexpr auto first_unset_element(const T& elemgr)
{
    const auto found = std::find(elemgr.begin_of_group, elemgr.end_of_group, decltype(*elemgr.begin_of_group){});
    return found == elemgr.end_of_group ? Element_Number<decltype(elemgr.tag)>{} : Element_Number<decltype(elemgr.tag)>{static_cast<int>(found - elemgr.origin)}; // narrowing is ok as long as you don't have more than 2 million elements
}

template <typename T>
constexpr auto number_of_set_elements(const T& elemgr)
{
    return std::count_if(elemgr.begin_of_group, elemgr.end_of_group, &is_set<decltype(*elemgr.begin_of_group)>);
}

template <typename T, typename U>
constexpr auto is_in_group(const T& elem, const U& elemgr)
{
    int first = elemgr.begin_of_group - elemgr.origin;
    int last = elemgr.end_of_group - elemgr.origin;
    return elem.v >= first && elem.v < last;
}

template <typename T>
auto all_unset_elements(const T& elemgr)
{
    std::vector<Element_Number<decltype(elemgr.tag)>> found;
    for (auto b = elemgr.begin_of_group; b != elemgr.end_of_group; b += 1) {
        if (is_unset(*b)) {
            found.push_back(Element_Number<decltype(elemgr.tag)>{static_cast<int>(b - elemgr.origin)}); // narrowing is ok as long as you don't have more than 2 million elements
        }
    }
    return found;
}

template <typename T, typename U>
auto filter_elements_in_group(const T& elemgr1, const U& elemgr2)
{
    std::vector<std::decay_t<decltype(*elemgr1.begin_of_group)>> found;
    int first = elemgr2.begin_of_group - elemgr2.origin;
    int last = elemgr2.end_of_group - elemgr2.origin;
    std::copy_if(elemgr1.begin_of_group, elemgr1.end_of_group, std::back_inserter(found), [first, last](auto elem) { return elem.v >= first && elem.v < last; });
    return found; // contains values of elemgr1 in the range of elemgr2
}

}

struct Piece_Tag {};
using Piece_Number = detail::Element_Number<const Piece_Tag>;
static constexpr Piece_Number no_piece; // default constructed means unset
using Piece_Range = detail::Element_Range<const Piece_Tag>;
template <typename T>
using Piecegroup = detail::Elementgroup<T, const Piece_Tag>;

template <typename T>
constexpr auto make_piecegroup(const T& pieces, const Piece_Number start, const Piece_Range range)
{
    return make_elementgroup(pieces, start, range);
}

struct Field_Tag {};
using Field_Number = detail::Element_Number<const Field_Tag>;
static constexpr Field_Number no_field; // default constructed means unset
using Field_Range = detail::Element_Range<const Field_Tag>;
template <typename T>
using Fieldgroup = detail::Elementgroup<T, const Field_Tag>;

void from_json(const nlohmann::json& j, boardgame::Field_Number& d);
void to_json(nlohmann::json& j, const Field_Number& d);

template <typename T>
constexpr auto make_fieldgroup(const T& fields, const Field_Number start, const Field_Range range)
{
    return make_elementgroup(fields, start, range);
}

template <typename T>
constexpr auto first_piece(const T& fieldgroup)
{
    return first_set_element(fieldgroup);
}

template <typename T>
constexpr bool is_fieldgroup_empty(const T& fieldgroup)
{
    return first_piece(fieldgroup) == no_piece;
}

template <typename T>
constexpr Field_Number first_empty_field(const T& fieldgroup)
{
    return first_unset_element(fieldgroup);
}

template <typename T>
constexpr int number_of_pieces_in_fieldgroup(const T& fieldgroup)
{
    return number_of_set_elements(fieldgroup);
}

template <typename T>
auto all_free_fields(const T& fieldgroup)
{
    return all_unset_elements(fieldgroup);
}

template <typename T, typename U>
auto positions_of_pieces_in_fieldgroup(const T& piecegroup, const U& fieldgroup)
{
    return filter_elements_in_group(piecegroup, fieldgroup);
}

using Field_Number_Diff = std::vector<std::array<Field_Number, 2>>;

}

#endif // SRC_BOARDGAME_PIECES_N_FIELDS
