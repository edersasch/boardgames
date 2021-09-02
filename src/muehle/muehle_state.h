#ifndef SRC_MUEHLE_MUEHLE_STATE
#define SRC_MUEHLE_MUEHLE_STATE

#include "muehle.h"
#include "boardgame/alpha_beta.h"
#include "boardgame/boardgame_ui.h"
#include "boardgame/move_list_ui.h"
#include "boardgame/pieces_n_fields.h"
#include "boardgame/move_list.h"
#include "boardgame/main_loop.h"

#include <array>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

/** Model setup and rule checking
 *
 * Field numbers:
 *
 *     0--------1--------2
 *     |        |        |
 *     |  3-----4-----5  |
 *     |  |     |     |  |
 *     |  |  6--7--8  |  |
 *     9-10-11    12-13-14
 *     |  | 15-16-17  |  |
 *     |  |     |     |  |
 *     |  18---19----20  |
 *     |        |        |
 *     21------22-------23
 *
 *     24 - 32: white drawer
 *     33 - 41: black drawer
 *     42 - 48: white prison
 *     49 - 55: black prison
 *
 * Piece numbers:
 *
 *     0 -  8: white pieces
 *     9 - 17: black pieces
*/

namespace muehle
{

class Fsm;

class Muehle_State
{
public:
    Muehle_State(boardgame::Boardgame_Ui bui, boardgame::Move_List_Ui mlu, boardgame::Main_Loop ml);
    void new_game();
    void request_setup_mode_active(bool is_active);
    void piece_removed(const boardgame::Piece_Number pn);
    void request_select_piece(const boardgame::Piece_Number pn);
    void request_occupy(const boardgame::Field_Number fn);
    void request_engine_active(const std::string& player_id, bool is_active);
    void request_set_move_and_branch(const std::int32_t move_id, const std::int32_t branch_id = -1);
    void request_cut_off(const std::int32_t move_id);
    void request_move_list_forward();
    void request_move_list_back();
    void request_move_list_import(const std::string& import, bool is_path = true);
    bool request_move_list_export(const std::string& path) const;
    std::string get_move_list_string() const;
    std::int32_t get_move_id() const { return move_list->get_current_move_id(); }
    std::int32_t get_branch_start_id() const { return move_list->get_current_branch_start_id(); }
    void set_engine_depth(std::int32_t depth);
    void set_engine_time(std::chrono::seconds time_in_s);
    void engine_move();
    void stop_engine(); // also fsm action
    void tick_1s();
    bool is_engine_running() const { return engine.is_running(); }
private:
    friend Fsm;
    muehle::Muehle_Constellation current_constellation;
    muehle::Muehle_Key current_key;
    std::array<boardgame::Piece_Number, muehle::number_of_fields.v> all_fields {};
    const boardgame::Boardgame_Ui boardgame_ui;
    const boardgame::Move_List_Ui move_list_ui;
    const boardgame::Main_Loop main_loop;
    std::int32_t move_list_hint {-1};
    /// hint data: 0 = potential piece that "moved" (only relevant for first move)
    std::unique_ptr<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>> move_list {};
    struct Player
    {
        const boardgame::Piecegroup<decltype(current_constellation.cbegin())>& piece_group;
        const boardgame::Fieldgroup<decltype(all_fields.cbegin())>& drawer_group;
        const boardgame::Fieldgroup<decltype(all_fields.cbegin())>& prison_group;
        std::string id;
        bool engine_active;
        std::chrono::milliseconds active_time {0};
    };
    boardgame::Field_Number piece_to_field(const boardgame::Piece_Number pn) const { return current_constellation.at(pn.v); }
    boardgame::Piece_Number field_to_piece(const boardgame::Field_Number fn) const { return boardgame::Piece_Number{all_fields.at(fn.v)}; }
    muehle::Muehle_Key make_key() const { return muehle::constellation_to_key(current_constellation, current_player == &white_player); }
    void set_field_helper(const boardgame::Piece_Number pn, const boardgame::Field_Number fn);
    void set_selected_piece(const boardgame::Piece_Number pn);
    void clear_selectable_pieces();
    void set_selectable_pieces(const std::vector<int>& pns);
    void set_removable_pieces(const std::vector<int>& fns);
    std::vector<int> occupiable_empty_board_fields() { return muehle::free_fields(make_key()); }
    void set_occupiable_fields(const std::vector<int>& fns);
    void set_occupiable_board_fields(const std::int32_t field_number);
    void set_occupiable_empty_board_fields();
    void set_occupiable_empty_fields();
    void check_hide_drawer() const;
    void check_show_prison() const;
    boardgame::Field_Number_Diff diff_key(Muehle_Key key) const;
    boardgame::Field_Number_Diff diff_keys(Muehle_Key oldk, Muehle_Key newk) const;
    void set_player_on_hint(const std::vector<int>& hint);
    void modified_if_not_start_constellation();
    void new_move_list();
    std::int32_t count_moves() const;
    const boardgame::Piecegroup<decltype(current_constellation.cbegin())> white_pieces { make_piecegroup(current_constellation, muehle::first_white_piece, muehle::number_of_pieces_per_player) };
    const boardgame::Piecegroup<decltype(current_constellation.cbegin())> black_pieces { make_piecegroup(current_constellation, muehle::first_black_piece, muehle::number_of_pieces_per_player) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> game_board { make_fieldgroup(all_fields, muehle::first_board_field, muehle::number_of_board_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> white_drawer { make_fieldgroup(all_fields, muehle::first_white_drawer_field, muehle::number_of_drawer_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> black_drawer { make_fieldgroup(all_fields, muehle::first_black_drawer_field, muehle::number_of_drawer_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> white_prison { make_fieldgroup(all_fields, muehle::first_white_prison_field, muehle::number_of_prison_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> black_prison { make_fieldgroup(all_fields, muehle::first_black_prison_field, muehle::number_of_prison_fields) };
    Player white_player {white_pieces, white_drawer, white_prison, white_id, false};
    Player black_player {black_pieces, black_drawer, black_prison, black_id, false};
    Player* current_player {&white_player};
    Player* opponent_player {&black_player};
    boardgame::Piece_Number selected_piece {};
    std::vector<boardgame::Piece_Number> selectable_pieces {};
    std::vector<boardgame::Field_Number> m_occupiable_fields {};
    boardgame::Alpha_Beta<Muehle_Key, Engine_Helper, Muehle_Move_Data, Muehle_Key_Hash> engine;
    std::string import_string {};
    bool import_string_is_path {true};
    std::int32_t move_id_to_set {boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>::invalid_id};
    std::int32_t branch_id_to_set {boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>::invalid_id};
    bool game_over {false};
    std::chrono::time_point<std::chrono::steady_clock> last_time_accounting;
    Fsm* fsm;

    // fsm actions
    void start_new_game();
    void enter_setup_mode();
    void exit_setup_mode();
    void select_game_piece(boardgame::Piece_Number pn);
    void select_setup_piece(boardgame::Piece_Number pn);
    void occupy_game(boardgame::Field_Number fn);
    void occupy_setup(boardgame::Field_Number fn);
    void remove_piece_and_finish_move(boardgame::Piece_Number pn);
    void import_move_list();
    void set_move_and_branch();
    void set_selectable_game_pieces();
    void finish_move();
    void finish_engine_move();
    void restore_last_constellation();
};

}

#endif // SRC_MUEHLE_MUEHLE_STATE
