#include "muehle_state.h"

#include <boost/sml.hpp>


namespace muehle
{

namespace sml = boost::sml;

static constexpr auto piece_selection                       = sml::state<class Piece_Selection>;
static constexpr auto destination_selection                 = sml::state<class Destination_Selection>;
static constexpr auto prisoner_selection                    = sml::state<class Prisoner_Selection>;
static constexpr auto engine_running                        = sml::state<class Engine_Running>;

struct Trigger_Make_Move {};
static constexpr auto trigger_make_move                     = sml::event<Trigger_Make_Move>;
struct Trigger_Start_Engine {};
static constexpr auto trigger_start_engine                  = sml::event<Trigger_Start_Engine>;
struct Trigger_Select_Piece
{
    boardgame::Piece_Number pn;
};
static constexpr auto trigger_select_piece                  = sml::event<Trigger_Select_Piece>;
struct Trigger_Occupy
{
    boardgame::Field_Number fn;
};
static constexpr auto trigger_occupy                        = sml::event<Trigger_Occupy>;
struct Trigger_Remove
{
    boardgame::Piece_Number pn;
};
static constexpr auto trigger_remove                        = sml::event<Trigger_Remove>;
struct Trigger_No_Removable_Piece {};
static constexpr auto trigger_no_removable_piece            = sml::event<Trigger_No_Removable_Piece>;
struct Trigger_Engine_Started {};
static constexpr auto trigger_engine_started                = sml::event<Trigger_Engine_Started>;
struct Trigger_Engine_Stopped {};
static constexpr auto trigger_engine_stopped                = sml::event<Trigger_Engine_Stopped>;

struct Set_Selectable_Game_Pieces
{
    void operator()(Fsm* handler) const;
};
static constexpr Set_Selectable_Game_Pieces set_selectable_game_pieces;
struct Select_Game_Piece
{
    template <typename EventT>
    void operator()(Fsm* handler, const EventT& e) const;
};
static constexpr Select_Game_Piece select_game_piece;
struct Occupy_Game
{
    template <typename EventT>
    void operator()(Fsm* handler, const EventT& e) const;
};
static constexpr Occupy_Game occupy_game;
struct Remove_Piece_And_Finish_Move
{
    template <typename EventT>
    void operator()(Fsm* handler, const EventT& e) const;
};
static constexpr Remove_Piece_And_Finish_Move remove_piece_and_finish_move;
struct Finish_Move
{
    void operator()(Fsm* handler) const;
};
static constexpr Finish_Move finish_move;
struct Finish_Engine_Move
{
    void operator()(Fsm* handler) const;
};
static constexpr Finish_Engine_Move finish_engine_move;
struct Restore_Last_Constellation
{
    void operator()(Fsm* handler) const;
};
static constexpr Restore_Last_Constellation restore_last_constellation;

struct Make_Move
{
    auto operator()() const noexcept
    {
        using namespace sml;
        return make_transition_table(
                 *piece_selection                       + sml::on_entry<_>                                              / set_selectable_game_pieces
                , piece_selection                       + trigger_select_piece                                          / select_game_piece                         = destination_selection
                , piece_selection                       + trigger_engine_started                                                                                    = engine_running
                , piece_selection                       + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , destination_selection                 + trigger_select_piece                                          / select_game_piece
                , destination_selection                 + trigger_occupy                                                / occupy_game                               = prisoner_selection
                , destination_selection                 + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , prisoner_selection                    + trigger_remove                                                / remove_piece_and_finish_move              = piece_selection
                , prisoner_selection                    + trigger_no_removable_piece                                    / finish_move                               = piece_selection
                , prisoner_selection                    + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , engine_running                        + trigger_engine_stopped                                        / finish_engine_move                        = piece_selection
                );
    }
};

static constexpr auto muehle_start                          = sml::state<class Muehle_Start>;
static constexpr auto move_list_import                      = sml::state<class Move_List_Import>;
static constexpr auto move_list_import_engine_stopping      = sml::state<class Move_List_Import_Engine_Stopping>;
static constexpr auto set_move_and_branch_engine_stopping   = sml::state<class Set_Move_And_Branch_Endinge_Stopping>;
static constexpr auto new_game_engine_stopping              = sml::state<class New_Game_Engine_Stopping>;
static constexpr auto setup_mode_engine_stopping            = sml::state<class Setup_Mode_Engine_Stopping>;
static constexpr auto setup_mode                            = sml::state<class Setup_Mode>;
static constexpr auto make_move                             = sml::state<Make_Move>;

struct Trigger_Setup_Mode
{
    bool is_engine_running;
};
static constexpr auto trigger_setup_mode                    = sml::event<Trigger_Setup_Mode>;
struct Trigger_Import_Move_List
{
    bool is_engine_running;
};
static constexpr auto trigger_import_move_list              = sml::event<Trigger_Import_Move_List>;
struct Trigger_Start_New_Game
{
    bool is_engine_running;
};
static constexpr auto trigger_start_new_game                = sml::event<Trigger_Start_New_Game>;
struct Trigger_Set_Move_And_Branch
{
    bool is_engine_running;
};
static constexpr auto trigger_set_move_and_branch           = sml::event<Trigger_Set_Move_And_Branch>;

struct Is_Engine_Running
{
    template <typename EventT>
    bool operator()(const EventT& e) const
    {
        return e.is_engine_running;
    }
};
static constexpr Is_Engine_Running is_engine_running;

struct Select_Setup_Piece
{
    template <typename EventT>
    void operator()(Fsm* handler, const EventT& e) const;
};
static constexpr Select_Setup_Piece select_setup_piece;
struct Occupy_Setup
{
    template <typename EventT>
    void operator()(Fsm* handler, const EventT& e) const;
};
static constexpr Occupy_Setup occupy_setup;
struct Start_New_Game
{
    void operator()(Fsm* handler) const;
};
static constexpr Start_New_Game start_new_game;
struct Import_Move_List
{
    void operator()(Fsm* handler) const;
};
static constexpr Import_Move_List import_move_list;
struct Set_Move_And_Branch
{
    void operator()(Fsm* handler) const;
};
static constexpr Set_Move_And_Branch set_move_and_branch;
struct Stop_Engine
{
    void operator()(Fsm* handler) const;
};
static constexpr Stop_Engine stop_engine;
struct Enter_Setup_Mode
{
    void operator()(Fsm* handler) const;
};
static constexpr Enter_Setup_Mode enter_setup_mode;
struct Exit_Setup_Mode
{
    void operator()(Fsm* handler) const;
};
static constexpr Exit_Setup_Mode exit_setup_mode;

struct Transition_Table
{
    auto operator()() const noexcept
    {
        using namespace sml;
        return make_transition_table(
                 *muehle_start                          + trigger_import_move_list                                                                                  = move_list_import
                , muehle_start                          + trigger_start_new_game                                        / start_new_game                            = make_move
                , move_list_import                      + sml::on_entry<_>                                              / import_move_list
                , move_list_import                      + trigger_start_new_game                                        / start_new_game                            = make_move
                , move_list_import                      + trigger_set_move_and_branch                                   / set_move_and_branch                       = make_move
                , make_move                             + trigger_setup_mode                  [ is_engine_running ]     / stop_engine                               = setup_mode_engine_stopping
                , make_move                             + trigger_setup_mode                  [ !is_engine_running ]                                                = setup_mode
                , make_move                             + trigger_start_new_game              [ is_engine_running ]     / stop_engine                               = new_game_engine_stopping
                , make_move                             + trigger_start_new_game              [ !is_engine_running ]    / start_new_game                            = make_move
                , make_move                             + trigger_import_move_list            [ is_engine_running ]     / stop_engine                               = move_list_import_engine_stopping
                , make_move                             + trigger_import_move_list            [ !is_engine_running ]                                                = move_list_import
                , make_move                             + trigger_set_move_and_branch         [ is_engine_running ]     / stop_engine                               = set_move_and_branch_engine_stopping
                , make_move                             + trigger_set_move_and_branch         [ !is_engine_running ]    / set_move_and_branch                       = make_move
                , setup_mode                            + sml::on_entry<_>                                              / enter_setup_mode
                , setup_mode                            + trigger_select_piece                                          / select_setup_piece
                , setup_mode                            + trigger_occupy                                                / occupy_setup
                , setup_mode                            + trigger_make_move                                                                                         = make_move
                , setup_mode                            + trigger_start_new_game                                        / start_new_game                            = make_move
                , setup_mode                            + trigger_import_move_list                                                                                  = move_list_import
                , setup_mode                            + sml::on_exit<_>                                               / exit_setup_mode
                , new_game_engine_stopping              + trigger_engine_stopped                                        / start_new_game                            = make_move
                , move_list_import_engine_stopping      + trigger_engine_stopped                                                                                    = move_list_import
                , set_move_and_branch_engine_stopping   + trigger_engine_stopped                                        / set_move_and_branch                       = make_move
                , setup_mode_engine_stopping            + trigger_engine_stopped                                                                                    = setup_mode
                );
    }
};

struct Fsm
{
    Fsm(Muehle_State* action_handler) : ah(action_handler), fsm{this} {}
    sml::sm<Transition_Table>& get_fsm()                                            { return fsm; }
    void start_new_game()                                                           { ah->start_new_game(); }
    void stop_engine()                                                              { ah->stop_engine(); }
    void enter_setup_mode()                                                         { ah->enter_setup_mode(); }
    void exit_setup_mode()                                                          { ah->exit_setup_mode(); }
    void set_selectable_game_pieces()                                               { ah->set_selectable_game_pieces(); }
    void select_game_piece(boardgame::Piece_Number pn)                              { ah->select_game_piece(pn); }
    void select_setup_piece(boardgame::Piece_Number pn)                             { ah->select_setup_piece(pn); }
    void occupy_setup(boardgame::Field_Number fn)                                   { ah->occupy_setup(fn); }
    void occupy_game(boardgame::Field_Number fn)                                    { ah->occupy_game(fn); }
    void remove_piece_and_finish_move(boardgame::Piece_Number pn)                   { ah->remove_piece_and_finish_move(pn); }
    void finish_move()                                                              { ah->finish_move(); }
    void finish_engine_move()                                                       { ah->finish_engine_move(); }
    void import_move_list()                                                         { ah->import_move_list(); }
    void set_move_and_branch()                                                      { ah->set_move_and_branch(); }
    void restore_last_constellation()                                               { ah->restore_last_constellation(); }
private:
    Muehle_State* ah;
    sml::sm<Transition_Table> fsm;
};

void Start_New_Game::operator()(Fsm *handler) const                                 { handler->start_new_game(); }
void Stop_Engine::operator()(Fsm* handler) const                                    { handler->stop_engine(); }
void Enter_Setup_Mode::operator()(Fsm* handler) const                               { handler->enter_setup_mode(); }
void Exit_Setup_Mode::operator()(Fsm* handler) const                                { handler->exit_setup_mode(); }
void Set_Selectable_Game_Pieces::operator()(Fsm* handler) const                     { handler->set_selectable_game_pieces(); }
template <typename EventT>
void Select_Game_Piece::operator()(Fsm* handler, const EventT& e) const             { handler->select_game_piece(e.pn); }
template <typename EventT>
void Select_Setup_Piece::operator()(Fsm* handler, const EventT& e) const            { handler->select_setup_piece(e.pn); }
template <typename EventT>
void Occupy_Setup::operator()(Fsm* handler, const EventT& e) const                  { handler->occupy_setup(e.fn); }
template <typename EventT>
void Occupy_Game::operator()(Fsm* handler, const EventT& e) const                   { handler->occupy_game(e.fn); }
template <typename EventT>
void Remove_Piece_And_Finish_Move::operator()(Fsm* handler, const EventT& e) const  { handler->remove_piece_and_finish_move(e.pn); }
void Finish_Move::operator()(Fsm* handler) const                                    { handler->finish_move(); }
void Finish_Engine_Move::operator()(Fsm* handler) const                             { handler->finish_engine_move(); }
void Import_Move_List::operator()(Fsm* handler) const                               { handler->import_move_list(); }
void Set_Move_And_Branch::operator()(Fsm* handler) const                            { handler->set_move_and_branch(); }
void Restore_Last_Constellation::operator()(Fsm *handler) const                     { handler->restore_last_constellation(); }

Muehle_State::Muehle_State(boardgame::Boardgame_Ui bui, boardgame::Move_List_Ui mlu, boardgame::Main_Loop ml)
    : boardgame_ui(std::move(bui))
    , move_list_ui(std::move(mlu))
    , main_loop(std::move(ml))
    , last_time_accounting(std::chrono::steady_clock::now())
    , fsm(new Fsm(this))
{
}

void Muehle_State::new_game()
{
    fsm->get_fsm().process_event(Trigger_Start_New_Game{engine.is_running()});
}

void Muehle_State::request_setup_mode_active(bool is_active)
{
    if (is_active) {
        fsm->get_fsm().process_event(Trigger_Setup_Mode{engine.is_running()});
    } else {
        fsm->get_fsm().process_event(Trigger_Make_Move());
    }
}

void Muehle_State::piece_removed(const boardgame::Piece_Number pn)
{
    if (std::find(selectable_pieces.begin(), selectable_pieces.end(), pn) != selectable_pieces.end()) {
        fsm->get_fsm().process_event(Trigger_Remove{pn});
    }
}

void Muehle_State::request_select_piece(const boardgame::Piece_Number pn)
{
    if (std::find(selectable_pieces.begin(), selectable_pieces.end(), pn) != selectable_pieces.end() && pn != selected_piece) { // always false in phase 1
        fsm->get_fsm().process_event(Trigger_Select_Piece{pn});
    }
}

void Muehle_State::request_occupy(const boardgame::Field_Number fn)
{
    if (selected_piece != boardgame::no_piece && std::find(m_occupiable_fields.begin(), m_occupiable_fields.end(), fn) != m_occupiable_fields.end()) {
        fsm->get_fsm().process_event(Trigger_Occupy{fn});
    }
}

void Muehle_State::request_engine_active(const std::string& player_id, bool is_active)
{
    Player* player = nullptr;
    if (player_id == white_id) {
        player = &white_player;
    } else if (player_id == black_id) {
        player = &black_player;
    } else {
        return;
    }
    if (player->engine_active != is_active) {
        player->engine_active = is_active;
        if (is_active) {
            fsm->get_fsm().process_event(Trigger_Make_Move());
            if (current_player == player) {
                fsm->get_fsm().process_event(Trigger_Start_Engine());
            }
        } else {
            if (current_player == player) {
                engine.stop_running();
            }
        }
    }
    engine_active(boardgame_ui, player_id, is_active);
}

void Muehle_State::request_set_move_and_branch(const int move_id, const int branch_id)
{
    if (move_id != -1) {
        move_id_to_set = move_id;
        branch_id_to_set = branch_id;
        fsm->get_fsm().process_event(Trigger_Set_Move_And_Branch{engine.is_running()});
    }
}

void Muehle_State::request_cut_off(const int move_id)
{
    int currm = move_list->get_current_move_id();
    int currb = move_list->get_current_branch_start_id();
    if (move_list->cut_off(move_id) &&
            (currm != move_list->get_current_move_id() ||
             currb != move_list->get_current_branch_start_id())) {
        request_set_move_and_branch(move_list->get_current_move_id(), move_list->get_current_branch_start_id());
    }
}

void Muehle_State::request_move_list_forward()
{
    if (move_list->move_list_forward()) {
        request_set_move_and_branch(move_list->get_current_move_id(), move_list->get_current_branch_start_id());
    }
}

void Muehle_State::request_move_list_back()
{
    if (move_list->move_list_back()) {
        request_set_move_and_branch(move_list->get_current_move_id(), move_list->get_current_branch_start_id());
    }
}

void Muehle_State::request_move_list_import(const std::string& import, bool is_path)
{
    import_string = import;
    import_string_is_path = is_path;
    fsm->get_fsm().process_event(Trigger_Import_Move_List{engine.is_running()});
}

bool Muehle_State::request_move_list_export(const std::string& path) const
{
    return move_list->export_move_list(path);
}

std::string Muehle_State::get_move_list_string() const
{
    return move_list->get_move_list_string();
}

void Muehle_State::set_engine_depth(int depth)
{
    if (depth) {
        set_engine_time(std::chrono::seconds(0)); // do not limit by time if depth gets set
    }
    engine.set_target_depth(depth);
}

void Muehle_State::set_engine_time(std::chrono::seconds time_in_s)
{
    if (time_in_s > std::chrono::seconds(0)) {
        set_engine_depth(0); // do not limit by depth if time gets set
    }
    set_main_loop_engine_time(main_loop, time_in_s);
}

void Muehle_State::engine_move()
{
    engine_forecast(boardgame_ui, 0, 0, std::vector<std::string>()); // clear forecast to prevent showing wrong colors
    fsm->get_fsm().process_event(Trigger_Engine_Stopped());
}

void Muehle_State::stop_engine()
{
    engine.stop_running();
}

void Muehle_State::tick_1s()
{
    auto new_last = std::chrono::steady_clock::now();
    if (!game_over) {
        std::chrono::milliseconds time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(new_last - last_time_accounting);
        current_player->active_time += time_diff;
        player_time(boardgame_ui, current_player->id, current_player->active_time);
    }
    last_time_accounting = new_last;
    if (current_player->engine_active) {
        std::vector<std::string> descriptions;
        auto curr = current_key;
        for (auto& n : engine.get_next()) {
            descriptions.push_back(muehle::diff_text(diff_keys(curr, n)));
            curr = n;
        }
        engine_forecast(boardgame_ui, engine.get_score(), engine.get_depth(), descriptions);
    }
}

// private

void Muehle_State::set_field_helper(const boardgame::Piece_Number pn, const boardgame::Field_Number fn)
{
    if (const boardgame::Field_Number oldfield = piece_to_field(pn); oldfield != boardgame::no_field) {
        all_fields.at(static_cast<std::size_t>(oldfield.v)) = boardgame::no_piece;
    }
    current_constellation.at(static_cast<std::size_t>(pn.v)) = fn;
    all_fields.at(static_cast<std::size_t>(fn.v)) = pn;
    set_field(boardgame_ui, pn, fn);
    if (game_over) {
        lock_piece(boardgame_ui, pn);
    }
}

void Muehle_State::set_selected_piece(const boardgame::Piece_Number pn)
{
    if (selected_piece != boardgame::no_piece) {
        lock_field(boardgame_ui, piece_to_field(selected_piece)); // ui can stop to highlight start position
        if (pn == boardgame::no_piece) {
            lock_piece(boardgame_ui, selected_piece);
        }
    }
    selected_piece = pn;
    if (selected_piece != boardgame::no_piece) {
        highlight(boardgame_ui, piece_to_field(selected_piece));
        selectable(boardgame_ui, selected_piece);
    }
}

void Muehle_State::clear_selectable_pieces()
{
    for (auto sp : selectable_pieces) {
        lock_piece(boardgame_ui, sp);
    }
    selectable_pieces.clear();
}

void Muehle_State::set_selectable_pieces(const std::vector<int>& pns)
{
    clear_selectable_pieces();
    for (auto field_of_selectable_piece : pns) {
        selectable_pieces.push_back(field_to_piece(boardgame::Field_Number{field_of_selectable_piece}));
        selectable(boardgame_ui, selectable_pieces.back());
    }
}

void Muehle_State::set_removable_pieces(const std::vector<int>& fns)
{
    clear_selectable_pieces();
    for (auto field_of_removable_piece : fns) {
        selectable_pieces.push_back(field_to_piece(boardgame::Field_Number{field_of_removable_piece}));
        removable(boardgame_ui, selectable_pieces.back());
    }
}

void Muehle_State::set_occupiable_fields(const std::vector<int>& fns)
{
    for (auto of : m_occupiable_fields) {
        lock_field(boardgame_ui, of);
    }
    m_occupiable_fields.clear();
    for (auto fn : fns) {
        m_occupiable_fields.push_back(boardgame::Field_Number{fn});
        occupiable(boardgame_ui, boardgame::Field_Number{fn}, selected_piece);
    }
}

void Muehle_State::set_occupiable_board_fields(const int field_number)
{
    set_occupiable_fields(muehle::occupiable_fields(make_key(), field_number));
}

void Muehle_State::set_occupiable_empty_board_fields()
{
    set_occupiable_fields(occupiable_empty_board_fields());
}

void Muehle_State::set_occupiable_empty_fields()
{
    auto fns = occupiable_empty_board_fields();
    for (const auto& f : all_free_fields(current_player->drawer_group)) {
        fns.push_back(f.v);
    }
    for (const auto& f : all_free_fields(current_player->prison_group)) {
        fns.push_back(f.v);
    }
    set_occupiable_fields(fns);
}

void Muehle_State::check_hide_drawer() const
{
    if (is_fieldgroup_empty(current_player->drawer_group)) {
        drawer_can_hide(boardgame_ui, current_player->id, true);
    }
}

void Muehle_State::check_show_prison() const
{
    if (!is_fieldgroup_empty(current_player->prison_group)) {
        prison_can_hide(boardgame_ui, current_player->id, false);
    }
}

boardgame::Field_Number_Diff Muehle_State::diff_key(Muehle_Key key) const
{
    auto diffed = diff_keys(make_key(), key);
    for (auto& el : diffed) {
        if (el.at(0).v >= first_white_drawer_field.v) {
            el.at(0) = piece_to_field(boardgame::first_piece(key.test(use_white_data_in_key) ? black_player.drawer_group : white_player.drawer_group));
        }
        if (el.at(1).v >= first_white_prison_field.v) {
            el.at(1) = boardgame::first_empty_field(key.test(use_white_data_in_key) ? white_player.prison_group : black_player.prison_group);
        }
    }
    return diffed;
}

/// returns fist drawer / prison field, not first occupied / empty
boardgame::Field_Number_Diff Muehle_State::diff_keys(Muehle_Key oldk, Muehle_Key newk) const
{
    boardgame::Field_Number_Diff diffed;
    auto single_diff = [&oldk, &newk, &diffed]() {
        std::vector<int> single_from;
        std::vector<int> single_to;
        auto offset = board_offset(oldk);
        for (int i = 0; i < number_of_board_fields.v; i += 1) {
            if (oldk.test(i + offset) && !newk.test(i + offset)) {
                single_from.push_back(i);
            } else {
                if (!oldk.test(i + offset) && newk.test(i + offset)) {
                    single_to.push_back(i);
                }
            }
        }
        auto f = single_from.begin();
        auto t = single_to.begin();
        for (; f != single_from.end() && t != single_to.end(); f += 1, t += 1) {
            diffed.push_back({boardgame::Field_Number{*f}, boardgame::Field_Number{*t}});
        }
        long prison_transfer = prisoner_count(oldk) - (prisoner_count(newk.test(use_white_data_in_key) == oldk.test(use_white_data_in_key) ? newk : muehle::switch_player(newk)));
        auto next_free_prison_field = oldk.test(use_white_data_in_key) ? muehle::first_white_prison_field : muehle::first_black_prison_field;
        while (f != single_from.end() && prison_transfer < 0) {
            auto field = boardgame::Field_Number{*f};
            diffed.push_back({field, next_free_prison_field});
            f += 1;
            prison_transfer += 1;
        }
        while (t != single_to.end()) {
            auto field = newk.test(use_white_data_in_key) ? muehle::first_white_drawer_field : muehle::first_black_drawer_field;
            diffed.push_back({field, boardgame::Field_Number{*t}});
            t += 1;
        }
        // no check to move pieces out of prison or into drawer
    };
    oldk.set(use_white_data_in_key);
    single_diff();
    oldk.reset(use_white_data_in_key);
    single_diff();
    return diffed;
}

void Muehle_State::set_player_on_hint(const std::vector<int>& hint)
{
    current_player = &white_player;
    opponent_player = &black_player;
    if (!hint.empty()) {
        if (hint.front() < muehle::first_black_piece.v) {
            current_player = &black_player;
            opponent_player = &white_player;
        }
    }
}

void Muehle_State::modified_if_not_start_constellation()
{
    if (!move_list->is_empty() ||
            boardgame::first_empty_field(white_player.drawer_group) != boardgame::no_field ||
            boardgame::first_empty_field(black_player.drawer_group) != boardgame::no_field
            ) {
        move_list->set_modified();
    }
    white_player.active_time = std::chrono::milliseconds(0);
    black_player.active_time = std::chrono::milliseconds(0);
    last_time_accounting = std::chrono::steady_clock::now();
    time_accounting_correct(boardgame_ui, !move_list->is_modified());
    player_time(boardgame_ui, white_player.id, white_player.active_time);
    player_time(boardgame_ui, black_player.id, black_player.active_time);
}

void Muehle_State::new_move_list()
{
    move_list = std::make_unique<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>>(move_list_ui, muehle::diff_text, current_constellation,
    std::vector<int>{current_player == &white_player ? muehle::first_black_piece.v : muehle::first_white_piece.v}); // initial position, hint is the potential piece that "moved"
}

int Muehle_State::count_moves() const
{
    return move_list->count_predecessors_if([](std::vector<int> hint) -> bool {
        return hint[0] < muehle::first_black_piece.v;
    });
}

// fsm actions

void Muehle_State::start_new_game()
{
    request_engine_active(white_id, false);
    current_player = &white_player;
    opponent_player = &black_player;
    set_selected_piece(boardgame::no_piece); // stop highlighting last field of potential previous setup mode
    std::fill(all_fields.begin(), all_fields.end(), boardgame::no_piece);
    std::fill(current_constellation.begin(), current_constellation.end(), boardgame::no_field);
    for (int i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
        set_field_helper(boardgame::Piece_Number{muehle::first_white_piece.v + i}, boardgame::Field_Number{muehle::first_white_drawer_field.v + i});
        set_field_helper(boardgame::Piece_Number{muehle::first_black_piece.v + i}, boardgame::Field_Number{muehle::first_black_drawer_field.v + i});
    }
    new_move_list();
    exit_setup_mode();
}

void Muehle_State::enter_setup_mode()
{
    setup_mode_active(boardgame_ui, true);
    new_move_list(); // delete move list, so no button can be pressed any more
    move_list->set_modified();
    request_engine_active(white_id, false);
    request_engine_active(black_id, false);
    if (selected_piece == boardgame::no_piece) {
        set_selected_piece(current_player == &white_player ? muehle::first_white_piece : muehle::first_black_piece);
    }
    set_occupiable_empty_fields();
    std::vector<int> fosp;
    for (auto fn : current_constellation) {
        fosp.push_back(fn.v);
    }
    set_selectable_pieces(fosp);
    drawer_can_hide(boardgame_ui, white_id, false);
    drawer_can_hide(boardgame_ui, black_id, false);
    prison_can_hide(boardgame_ui, white_id, false);
    prison_can_hide(boardgame_ui, black_id, false);
}

void Muehle_State::exit_setup_mode()
{
    setup_mode_active(boardgame_ui, false);
    set_selected_piece(boardgame::no_piece);
    clear_selectable_pieces();
    set_occupiable_fields({});
    drawer_can_hide(boardgame_ui, white_id, is_fieldgroup_empty(white_drawer));
    drawer_can_hide(boardgame_ui, black_id, is_fieldgroup_empty(black_drawer));
    prison_can_hide(boardgame_ui, white_id, is_fieldgroup_empty(white_prison));
    prison_can_hide(boardgame_ui, black_id, is_fieldgroup_empty(black_prison));
    modified_if_not_start_constellation();
    need_confirm(boardgame_ui, move_list->is_modified());
}

void Muehle_State::select_game_piece(boardgame::Piece_Number pn)
{
    set_selected_piece(pn);
    if (muehle::game_phase(make_key()) < 3) {
        set_occupiable_board_fields(piece_to_field(selected_piece).v);
    }
}

void Muehle_State::select_setup_piece(boardgame::Piece_Number pn)
{
    set_selected_piece(pn);
    if (!is_in_group(selected_piece, current_player->piece_group)) {
        Player* tmp = current_player;
        current_player = opponent_player;
        opponent_player = tmp;
        new_move_list();
        set_occupiable_empty_fields();
    }
}

void Muehle_State::occupy_game(boardgame::Field_Number fn)
{
    auto from = piece_to_field(selected_piece).v >= muehle::first_white_drawer_field.v ? muehle::drawer_field : piece_to_field(selected_piece).v;
    auto key = make_key();
    auto piece = selected_piece;
    set_selected_piece(boardgame::no_piece); // do not highlight start field
    set_occupiable_fields({});
    set_field_helper(piece, fn);
    move_list_hint = piece.v;
    auto [fields_of_removable_pieces, successor] = muehle::occupy(key, from , fn.v);
    (void)successor;
    check_hide_drawer();
    if (fields_of_removable_pieces.empty()) {
        fsm->get_fsm().process_event(Trigger_No_Removable_Piece());
    } else {
        set_removable_pieces(fields_of_removable_pieces);
    }
}

void Muehle_State::occupy_setup(boardgame::Field_Number fn)
{
    auto piece = selected_piece;
    set_selected_piece(boardgame::no_piece); // do not highlight start field
    set_occupiable_fields({});
    set_field_helper(piece, fn);
    set_selected_piece(piece); // highlight new start field
    set_occupiable_empty_fields();
    new_move_list();
}

void Muehle_State::remove_piece_and_finish_move(boardgame::Piece_Number pn)
{
    prison_can_hide(boardgame_ui, opponent_player->id, false);
    set_field_helper(pn, first_empty_field(opponent_player->prison_group));
    finish_move();
}

void Muehle_State::import_move_list()
{
    request_engine_active(white_id, false);
    request_engine_active(black_id, false);
    move_list = std::make_unique<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>>(move_list_ui, muehle::diff_text, import_string, import_string_is_path);
    if (move_list->get_current_move_id() != 0) {
        fsm->get_fsm().process_event(Trigger_Start_New_Game{engine.is_running()});
    } else {
        request_set_move_and_branch(0);
    }
}

void Muehle_State::set_move_and_branch()
{
    if (move_list->set_current_move_and_branch_start_id(move_id_to_set, branch_id_to_set)) {
        request_engine_active(white_id, false);
        request_engine_active(black_id, false);
        set_selected_piece(boardgame::no_piece);
        std::fill(all_fields.begin(), all_fields.end(), boardgame::no_piece);
        std::fill(current_constellation.begin(), current_constellation.end(), boardgame::no_field);
        for (int i = 0; i < static_cast<int>(move_list->constellation().size()); i += 1) {
            set_field_helper(boardgame::Piece_Number{i}, move_list->constellation().at(static_cast<std::size_t>(i)));
        }
        set_player_on_hint(move_list->hint());
        exit_setup_mode();
    }
}

void Muehle_State::set_selectable_game_pieces()
{
    if (boardgame::first_empty_field(opponent_player->prison_group) == boardgame::no_field) { // fun with setup mode
        active_player(boardgame_ui, "");
        return;
    }
    movecount(boardgame_ui, count_moves());
    current_key = make_key();
    auto fosp = muehle::fields_of_selectable_pieces(current_key);
    if (fosp.empty() ||
            boardgame::first_empty_field(current_player->prison_group) == boardgame::no_field) {
        game_over = true;
        win(boardgame_ui, opponent_player->id);
        active_player(boardgame_ui, "");
        return;
    }
    active_player(boardgame_ui, current_player->id);
    if (!current_player->engine_active) {
        if (fosp.size() == 1) {
            if (fosp.front() == -1) {
                fosp.front() = piece_to_field(first_piece(current_player->drawer_group)).v;
            }
            fsm->get_fsm().process_event(Trigger_Select_Piece{field_to_piece(boardgame::Field_Number{fosp.front()})}); // no guard as in request_select_piece()
        } else {
            set_selectable_pieces(fosp);
            if (muehle::game_phase(current_key) == 3) {
                set_selected_piece(first_piece(current_player == &white_player ? white_player.prison_group : black_player.prison_group)); // for correct occupiables color, piece is not selectable
                set_occupiable_empty_board_fields();
                set_selected_piece(boardgame::no_piece);
            }
        }
    } else {
        fsm->get_fsm().process_event(Trigger_Engine_Started());
        set_selected_piece(boardgame::no_piece);
        engine_future(main_loop, std::async(std::launch::async, [this]() {
            engine.start(current_key);
        }));
    }
}

void Muehle_State::finish_move()
{
    auto old_dirty = move_list->is_modified();
    move_list->commit(current_constellation, {move_list_hint});
    if (move_list->is_modified() && !old_dirty) {
        need_confirm(boardgame_ui, move_list->is_modified());
    }
    check_hide_drawer();
    Player* tmp = current_player;
    current_player = opponent_player;
    opponent_player = tmp;
    check_show_prison();
    clear_selectable_pieces();
    set_selected_piece(boardgame::no_piece);
}

void Muehle_State::finish_engine_move()
{
    auto next_keys = engine.get_next();
    for (auto d : diff_key(next_keys.front())) {
        set_field_helper(field_to_piece(d.at(0)), d.at(1));
        if (is_in_group(d.at(1), game_board)) {
            move_list_hint = field_to_piece(d.at(1)).v;
        }
    }
    finish_move();
}

void Muehle_State::restore_last_constellation()
{
    for (std::size_t i = 0; i < current_constellation.size(); i += 1) {
        if (current_constellation.at(i) != move_list->constellation().at(i)) {
            set_field_helper(boardgame::Piece_Number{static_cast<int>(i)}, move_list->constellation().at(i));
        }
    }
    drawer_can_hide(boardgame_ui, current_player->id, boardgame::is_fieldgroup_empty(current_player->drawer_group));
    clear_selectable_pieces();
    set_occupiable_fields({});
}

}
