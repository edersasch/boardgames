#include "muehle_state.h"

namespace muehle
{

Muehle_State::Muehle_State(boardgame::Boardgame_Ui bui, boardgame::Move_List_Ui mlu, boardgame::Main_Loop ml)
    : boardgame_ui(std::move(bui))
    , move_list_ui(std::move(mlu))
    , main_loop(std::move(ml))
    , last_time_accounting(std::chrono::steady_clock::now())
{
}

void Muehle_State::new_game()
{
    if (engine.is_running()) {
        restart = true;
        engine.discard();
        request_engine_active(white_id, false); // necessary here becuase engine might run for black
    } else {
        request_engine_active(white_id, false); // necessary here because engine might not be running, but could become so in leave_setup_mode()
        restart = false;
        current_player = &white_player;
        opponent_player = &black_player;
        set_selected_piece(boardgame::no_piece); // stop highlighting last field of potential previous setup mode
        std::fill(all_fields.begin(), all_fields.end(), boardgame::no_piece);
        std::fill(current_constellation.begin(), current_constellation.end(), boardgame::no_field);
        for (int i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
            set_field_helper(boardgame::Piece_Number{muehle::first_white_piece.v + i}, boardgame::Field_Number{muehle::first_white_drawer_field.v + i});
            set_field_helper(boardgame::Piece_Number{muehle::first_black_piece.v + i}, boardgame::Field_Number{muehle::first_black_drawer_field.v + i});
        }
        setup_mode = true;
        leave_setup_mode();
    }
}

void Muehle_State::request_setup_mode_active(bool is_active)
{
    if (is_active) {
        if (!setup_mode) {
            enter_setup_mode();
        }
    } else {
        if (setup_mode) {
            leave_setup_mode();
        }
    }
}

void Muehle_State::piece_removed(const boardgame::Piece_Number pn)
{
    if (std::find(selectable_pieces.begin(), selectable_pieces.end(), pn) != selectable_pieces.end()) {
        prison_can_hide(boardgame_ui, opponent_player->id, false);
        set_field_helper(pn, first_empty_field(opponent_player->prison_group));
        swap_players();
    }
}

void Muehle_State::request_select_piece(const boardgame::Piece_Number pn)
{
    if (std::find(selectable_pieces.begin(), selectable_pieces.end(), pn) != selectable_pieces.end() && pn != selected_piece) { // always false in phase 1
        set_selected_piece(pn);
        if (setup_mode) {
            if (!is_in_group(selected_piece, current_player->piece_group)) {
                Player* tmp = current_player;
                current_player = opponent_player;
                opponent_player = tmp;
                set_occupiable_empty_fields();
            }
        } else {
            if (muehle::game_phase(make_key()) == 2) {
                set_occupiable_board_fields(piece_to_field(selected_piece).v);
            }
        }
    }
}

void Muehle_State::request_occupy(const boardgame::Field_Number fn)
{
    if (selected_piece == boardgame::no_piece || std::find(m_occupiable_fields.begin(), m_occupiable_fields.end(), fn) == m_occupiable_fields.end()) {
        return;
    }
    auto from = piece_to_field(selected_piece).v >= muehle::first_white_drawer_field.v ? muehle::drawer_field : piece_to_field(selected_piece).v;
    auto key = make_key();
    auto piece = selected_piece;
    set_selected_piece(boardgame::no_piece); // do not highlight start field
    set_occupiable_fields({});
    set_field_helper(piece, fn);
    if (setup_mode) {
        set_selected_piece(piece); // highlight new start field
        set_occupiable_empty_fields();
        return;
    }
    move_list_hint = piece.v;
    auto [fields_of_removable_pieces, successor] = muehle::occupy(key, from , fn.v);
    (void)successor;
    check_hide_drawer();
    if (fields_of_removable_pieces.empty()) {
        swap_players();
    } else {
        set_removable_pieces(fields_of_removable_pieces);
    }
}

void Muehle_State::request_engine_active(const std::string& player_id, bool is_active)
{
    if (player_id == white_id) {
        change_engine_active(&white_player, is_active);
    } else if (player_id == black_id) {
        change_engine_active(&black_player, is_active);
    } else {
        return;
    }
    engine_active(boardgame_ui, player_id, is_active);
}

void Muehle_State::request_set_current_move_and_branch_start_id(const int move_id)
{
    linear_game = (move_id == move_list->get_current_move_id() && linear_game);
    if (!setup_mode && move_list->set_current_move_and_branch_start_id(move_id)) {
        update_game();
    }
}

void Muehle_State::request_set_current_move_and_branch_start_id(const int move_id, const int branch_start_id)
{
    linear_game = (move_id == move_list->get_current_move_id() && linear_game);
    if (move_list->set_current_move_and_branch_start_id(move_id, branch_start_id)) {
        update_game();
    }
}

void Muehle_State::request_cut_off(const int move_id)
{
    if (move_list->cut_off(move_id)) {
        linear_game = false;
        update_game();
    }
}

void Muehle_State::request_move_list_forward()
{
    if (move_list->move_list_forward()) {
        linear_game = false;
        update_game();
    }
}

void Muehle_State::request_move_list_back()
{
    if (move_list->move_list_back()) {
        linear_game = false;
        update_game();
    }
}

void Muehle_State::request_move_list_import(const std::string& import, bool is_path)
{
    request_engine_active(white_id, false);
    request_engine_active(black_id, false);
    if (engine.is_running()) {
        import_string = import;
        import_string_is_path = is_path;
        engine.discard();
    } else {
        setup_mode = false;
        import_string.clear();
        setup_mode_active(boardgame_ui, false);
        move_list = std::make_unique<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>>(move_list_ui, muehle::diff_text, import, is_path);
        if (move_list->get_current_move_id() != 0) {
            new_game();
        } else {
            request_set_current_move_and_branch_start_id(move_list->get_current_move_id());
            if (is_path) { // if imported from path, the constellation is stored somewhere, so no confirmation needed
                clear_accounting();
            } else {
                modified_if_not_start_constellation();
            }
            need_confirm(boardgame_ui, move_list->is_modified());
        }
    }
}

bool Muehle_State::request_move_list_export(const std::string& path)
{
    if (setup_mode) {
        leave_setup_mode(); // commit current constellation
        enter_setup_mode();
    }
    return move_list->export_move_list(path);
}

std::string Muehle_State::get_move_list_string()
{
    if (setup_mode) {
        leave_setup_mode(); // commit current constellation
        enter_setup_mode();
    }
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

void Muehle_State::engine_move(bool is_valid)
{
    auto next_keys = engine.get_next();
    engine_forecast(boardgame_ui, 0, 0, std::vector<std::string>()); // clear forecast to prevent showing wrong colors
    if (is_valid) {
        for (auto d : diff_key(next_keys.front())) {
            set_field_helper(field_to_piece(d.at(0)), d.at(1));
            if (is_in_group(d.at(1), game_board)) {
                move_list_hint = field_to_piece(d.at(1)).v;
            }
        }
        swap_players();
    } else {
        if (restart) {
            new_game();
        } else if (setup_mode) {
            enter_setup_mode();
        } else if (!import_string.empty()) {
            request_move_list_import(import_string, import_string_is_path);
        } else {
            linear_game = false;
            update_game();
        }
    }
}

void Muehle_State::force_engine_move()
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

void Muehle_State::start_move()
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
            set_selected_piece(field_to_piece(boardgame::Field_Number{fosp.front()}));
            set_occupiable_board_fields(fosp.front());
        } else {
            set_selectable_pieces(fosp);
            if (muehle::game_phase(current_key) == 3) {
                set_selected_piece(first_piece(current_player == &white_player ? white_player.prison_group : black_player.prison_group)); // for correct occupiables color, piece ist not selectable
                set_occupiable_empty_board_fields();
                set_selected_piece(boardgame::no_piece);
            }
        }
    } else {
        engine_future(main_loop, std::async(std::launch::async, [this]() {
            return engine.start(current_key);
        }));
    }
}

void Muehle_State::swap_players()
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
    set_selectable_pieces({});
    set_selected_piece(boardgame::no_piece);
    start_move();
}

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

void Muehle_State::update_game()
{
    if (!engine.is_running()) {
        request_engine_active(white_id, false);
        request_engine_active(black_id, false);
        set_selected_piece(boardgame::no_piece);
        reconstruct(move_list->constellation());
        set_player_on_hint(move_list->hint());
        leave_setup_mode();
        time_accounting_correct(boardgame_ui, linear_game);
    } else {
        engine.discard();
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

void Muehle_State::change_engine_active(Player* player, const bool is_active)
{
    player->engine_active = is_active;
    if (is_active && (current_player == player || setup_mode)) {
        leave_setup_mode();
    } else {
        if (!is_active && current_player == player) {
            engine.stop_running();
        }
    }
}

void Muehle_State::enter_setup_mode()
{
    setup_mode = true;
    setup_mode_active(boardgame_ui, true);
    if (engine.is_running()) {
        engine.discard();
    } else {
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
}

void Muehle_State::leave_setup_mode()
{
    auto sm = setup_mode;
    setup_mode = false;
    game_over = false;
    if (sm) {
        setup_mode_active(boardgame_ui, false);
        new_move_list();
        modified_if_not_start_constellation();
        need_confirm(boardgame_ui, move_list->is_modified());
    }
    set_selected_piece(boardgame::no_piece);
    set_selectable_pieces({});
    set_occupiable_fields({});
    drawer_can_hide(boardgame_ui, white_id, is_fieldgroup_empty(white_drawer));
    drawer_can_hide(boardgame_ui, black_id, is_fieldgroup_empty(black_drawer));
    prison_can_hide(boardgame_ui, white_id, is_fieldgroup_empty(white_prison));
    prison_can_hide(boardgame_ui, black_id, is_fieldgroup_empty(black_prison));
    start_move();
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
        long prison_transfer = prisoner_count(oldk) - (prisoner_count(newk.test(use_white_data_in_key) == oldk.test(use_white_data_in_key) ? newk : muehle::Engine_Helper::switch_player(newk)));
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

void Muehle_State::reconstruct(const Muehle_Constellation& constellation)
{
    std::fill(all_fields.begin(), all_fields.end(), boardgame::no_piece);
    std::fill(current_constellation.begin(), current_constellation.end(), boardgame::no_field);
    for (int i = 0; i < static_cast<int>(constellation.size()); i += 1) {
        set_field_helper(boardgame::Piece_Number{i}, constellation.at(static_cast<std::size_t>(i)));
    }
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
    linear_game = true;
    if (!move_list->is_empty() ||
            boardgame::first_empty_field(white_player.drawer_group) != boardgame::no_field ||
            boardgame::first_empty_field(black_player.drawer_group) != boardgame::no_field
            ) {
        move_list->set_modified();
    }
    if (move_list->is_modified()) {
        linear_game = false;
    }
    clear_accounting();
}

void Muehle_State::clear_accounting()
{
    white_player.active_time = std::chrono::milliseconds(0);
    black_player.active_time = std::chrono::milliseconds(0);
    last_time_accounting = std::chrono::steady_clock::now();
    time_accounting_correct(boardgame_ui, linear_game);
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

}
