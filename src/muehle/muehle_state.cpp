#include "muehle_state.h"

namespace muehle
{

void Muehle_State::new_game()
{
    if (engine.is_running()) {
        restart = true;
        engine.discard();
        request_white_engine_active(false); // necessary here becuase engine might run for black
    } else {
        request_white_engine_active(false); // necessary here because engine might not be running, but could become so in leave_setup_mode()
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
        opponent_player == &white_player ?
                    white_prison_can_hide(boardgame_ui, false) :
                    black_prison_can_hide(boardgame_ui, false);
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
                const Player* tmp = current_player;
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

void Muehle_State::request_white_engine_active(bool is_active)
{
    change_engine_active(&white_player, is_active);
    white_engine_active(boardgame_ui, is_active);
}

void Muehle_State::request_black_engine_active(bool is_active)
{
    change_engine_active(&black_player, is_active);
    black_engine_active(boardgame_ui, is_active);
}

void Muehle_State::request_set_current_move_and_branch_start_id(const int move_id)
{
    if (move_list->set_current_move_and_branch_start_id(move_id)) {
        update_game();
    }
}

void Muehle_State::request_cut_off(const int move_id)
{
    if (move_list->cut_off(move_id)) {
        update_game();
    }
}

void Muehle_State::request_move_list_forward()
{
    if (move_list->move_list_forward()) {
        update_game();
    }
}

void Muehle_State::request_move_list_back()
{
    if (move_list->move_list_back()) {
        update_game();
    }
}

void Muehle_State::request_move_list_import(const std::string& path)
{
    request_white_engine_active(false);
    request_black_engine_active(false);
    if (engine.is_running()) {
        import_path = path;
        engine.discard();
    } else {
        import_path.clear();
        move_list = std::make_unique<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>>(move_list_ui, muehle::diff_text, path);
        if (move_list->get_current_move_id() != 0) {
            new_game();
        } else {
            request_set_current_move_and_branch_start_id(move_list->get_current_move_id());
        }
    }
}

bool Muehle_State::request_move_list_export(const std::string& path)
{
    return move_list->export_move_list(path);
}

void Muehle_State::engine_move(bool is_valid)
{
    if (is_valid) {
        for (auto d : diff_key(engine.get_next())) {
            set_field_helper(field_to_piece(d.at(0)), d.at(1));
            if (is_in_group(d.at(1), game_board)) {
                move_list_hint = field_to_piece(d.at(1)).v;
            }
        }
        swap_players();
    } else {
        if (restart) {
            new_game();
        } else {
            if (setup_mode) {
                enter_setup_mode();
            } else {
                if (!import_path.empty()) {
                    request_move_list_import(import_path);
                } else {
                    update_game();
                }
            }
        }
    }
}

void Muehle_State::force_engine_move()
{
    engine.stop_running();
}

// private

void Muehle_State::start_move()
{
    active_player(boardgame_ui, current_player->id);
    auto key = make_key();
    auto fosp = muehle::fields_of_selectable_pieces(key);
    if (fosp.empty()) {
        win(boardgame_ui, opponent_player->id);
        active_player(boardgame_ui, "");
        return;
    }
    if (!current_player->engine_active) {
        if (fosp.size() == 1) {
            if (fosp.front() == -1) {
                fosp.front() = piece_to_field(first_piece(current_player->drawer_group)).v;
            }
            set_selected_piece(field_to_piece(boardgame::Field_Number{fosp.front()}));
            set_occupiable_board_fields(fosp.front());
        } else {
            set_selectable_pieces(fosp);
            if (muehle::game_phase(key) == 3) {
                set_selected_piece(first_piece(current_player == &white_player ? white_player.prison_group : black_player.prison_group)); // for correct occupiables color, piece ist not selectable
                set_occupiable_empty_board_fields();
                set_selected_piece(boardgame::no_piece);
            }
        }
    } else {
        current_key = muehle::constellation_to_key(current_constellation, opponent_player == &white_player);
        engine_future(main_loop, std::async(std::launch::async, [this]() {
            return engine.start(current_key);
        }));
    }
}

void Muehle_State::swap_players()
{
    move_list->commit(current_constellation, {move_list_hint});
    if (first_empty_field(opponent_player->prison_group) == boardgame::no_field) {
        set_removable_pieces({});
        win(boardgame_ui, current_player->id);
        active_player(boardgame_ui, "");
    } else {
        check_hide_drawer();
        const Player* tmp = current_player;
        current_player = opponent_player;
        opponent_player = tmp;
        check_show_prison();
        set_selectable_pieces({});
        set_selected_piece(boardgame::no_piece);
        start_move();
    }
}

void Muehle_State::set_field_helper(const boardgame::Piece_Number pn, const boardgame::Field_Number fn)
{
    if (const boardgame::Field_Number oldfield = piece_to_field(pn); oldfield != boardgame::no_field) {
        all_fields.at(static_cast<std::size_t>(oldfield.v)) = boardgame::no_piece;
    }
    current_constellation.at(static_cast<std::size_t>(pn.v)) = fn;
    all_fields.at(static_cast<std::size_t>(fn.v)) = pn;
    set_field(boardgame_ui, pn, fn);
}

void Muehle_State::update_game()
{
    if (!engine.is_running()) {
        request_white_engine_active(false);
        request_black_engine_active(false);
        set_selected_piece(boardgame::no_piece);
        reconstruct(move_list->constellation());
        set_player_on_hint(move_list->hint());
        leave_setup_mode();
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

void Muehle_State::set_selectable_pieces(const std::vector<int>& fns)
{
    clear_selectable_pieces();
    for (auto field_of_selectable_piece : fns) {
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
        request_white_engine_active(false);
        request_black_engine_active(false);
        if (selected_piece == boardgame::no_piece) {
            set_selected_piece(current_player == &white_player ? muehle::first_white_piece : muehle::first_black_piece);
        }
        set_occupiable_empty_fields();
        std::vector<int> fosp;
        for (auto fn : current_constellation) {
            fosp.push_back(fn.v);
        }
        set_selectable_pieces(fosp);
        white_drawer_can_hide(boardgame_ui, false);
        white_prison_can_hide(boardgame_ui, false);
        black_drawer_can_hide(boardgame_ui, false);
        black_prison_can_hide(boardgame_ui, false);
    }
}

void Muehle_State::leave_setup_mode()
{
    auto sm = setup_mode;
    setup_mode = false;
    if (sm) {
        setup_mode_active(boardgame_ui, false);
        move_list = std::make_unique<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>>(move_list_ui, muehle::diff_text, current_constellation,
            std::vector<int>{current_player == &white_player ? muehle::first_black_piece.v : muehle::first_white_piece.v}); // initial position, hint is the potential piece that "moved"
    }
    set_selected_piece(boardgame::no_piece);
    set_selectable_pieces({});
    set_occupiable_fields({});
    white_drawer_can_hide(boardgame_ui, is_fieldgroup_empty(white_drawer));
    black_drawer_can_hide(boardgame_ui, is_fieldgroup_empty(black_drawer));
    white_prison_can_hide(boardgame_ui, is_fieldgroup_empty(white_prison));
    black_prison_can_hide(boardgame_ui, is_fieldgroup_empty(black_prison));
    start_move();
}

void Muehle_State::check_hide_drawer()
{
    if (is_fieldgroup_empty(current_player->drawer_group)) {
        current_player == &white_player ? white_drawer_can_hide(boardgame_ui, true) : black_drawer_can_hide(boardgame_ui, true);
    }
}

void Muehle_State::check_show_prison()
{
    if (!is_fieldgroup_empty(current_player->prison_group)) {
        current_player == &white_player ? white_prison_can_hide(boardgame_ui, false) : black_prison_can_hide(boardgame_ui, false);
    }
}

boardgame::Field_Number_Diff Muehle_State::diff_key(Muehle_Key key)
{
    boardgame::Field_Number_Diff diffed;
    auto single_diff = [this, &key, &diffed]() {
        std::vector<int> single_from;
        std::vector<int> single_to;
        std::vector<boardgame::Field_Number> fn = boardgame::positions_of_pieces_in_fieldgroup(key.test(54) ? white_player.piece_group : black_player.piece_group, game_board);
        auto offset = board_offset(key);
        for (int i = 0; i < number_of_board_fields.v; i += 1) {
            if ((std::find(fn.begin(), fn.end(), boardgame::Field_Number{i}) != fn.end()) && !key.test(i + offset)) {
                single_from.push_back(i);
            } else {
                if (key.test(i + offset) && std::find(fn.begin(), fn.end(), boardgame::Field_Number{i}) == fn.end()) {
                    single_to.push_back(i);
                }
            }
        }
        auto f = single_from.begin();
        auto t = single_to.begin();
        for (; f != single_from.end() && t != single_to.end(); f += 1, t += 1) {
            diffed.push_back({boardgame::Field_Number{*f}, boardgame::Field_Number{*t}});
        }
        int prison_transfer = number_of_pieces_in_fieldgroup(key.test(54) ? white_player.prison_group : black_player.prison_group) - prisoner_count(key);
        auto next_free_prison_field = boardgame::first_empty_field(key.test(54) ? white_player.prison_group : black_player.prison_group);
        while (f != single_from.end() && prison_transfer < 0) {
            auto field = boardgame::Field_Number{*f};
            diffed.push_back({field, next_free_prison_field});
            f += 1;
            prison_transfer += 1;
            next_free_prison_field = boardgame::first_empty_field(boardgame::make_fieldgroup(all_fields, boardgame::Field_Number{next_free_prison_field.v + 1}, boardgame::Field_Range{number_of_prison_fields.v - (next_free_prison_field.v + 1)}));
        }
        while (t != single_to.end()) {
            auto p = boardgame::first_piece(key.test(54) ? white_player.drawer_group : black_player.drawer_group);
            diffed.push_back({piece_to_field(p), boardgame::Field_Number{*t}});
            t += 1;
        }
        // no check to move pieces out of prison or into drawer
    };
    key.set(54);
    single_diff();
    key.reset(54);
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

}
