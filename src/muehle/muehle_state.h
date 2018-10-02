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

class Muehle_State
{
public:
    Muehle_State(boardgame::Boardgame_Ui bui, boardgame::Move_List_Ui mlu, boardgame::Main_Loop ml) : boardgame_ui(std::move(bui)), move_list_ui(std::move(mlu)), main_loop(std::move(ml)) {}
    void new_game();
    void request_setup_mode_active(bool is_active);
    void piece_removed(const boardgame::Piece_Number pn);
    void request_select_piece(const boardgame::Piece_Number pn);
    void request_occupy(const boardgame::Field_Number fn);
    void request_white_engine_active(bool is_active);
    void request_black_engine_active(bool is_active);
    void request_set_current_move_and_branch_start_id(const int move_id);
    void request_cut_off(const int move_id);
    void request_move_list_forward();
    void request_move_list_back();
    void engine_move(bool is_valid);
    void force_engine_move();
private:
    muehle::Muehle_Constellation current_constellation;
    muehle::Muehle_Key current_key;
    std::array<boardgame::Piece_Number, muehle::number_of_fields.v> all_fields {};
    const boardgame::Boardgame_Ui boardgame_ui;
    const boardgame::Move_List_Ui move_list_ui;
    const boardgame::Main_Loop main_loop;
    std::unique_ptr<boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>> move_list {new boardgame::Move_List<muehle::Muehle_Constellation, boardgame::Move_List_Ui>(move_list_ui)};
    struct Player
    {
        const boardgame::Piecegroup<decltype(current_constellation.cbegin())>& piece_group;
        const boardgame::Fieldgroup<decltype(all_fields.cbegin())>& drawer_group;
        const boardgame::Fieldgroup<decltype(all_fields.cbegin())>& prison_group;
        std::string id;
        bool engine_active;
    };
    boardgame::Field_Number piece_to_field(const boardgame::Piece_Number pn) { return current_constellation.at(pn.v); }
    boardgame::Piece_Number field_to_piece(const boardgame::Field_Number fn) { return boardgame::Piece_Number{all_fields.at(fn.v)}; }
    muehle::Muehle_Key make_key() { return muehle::constellation_to_key(current_constellation, current_player == &white_player); }
    void start_move();
    void swap_players();
    void set_field_helper(const boardgame::Piece_Number pn, const boardgame::Field_Number fn);
    void update_game();
    void set_selected_piece(const boardgame::Piece_Number pn);
    void clear_selectable_pieces();
    void set_selectable_pieces(const std::vector<int>& pns);
    void set_removable_pieces(const std::vector<int>& fns);
    std::vector<int> occupiable_empty_board_fields() { return muehle::free_fields(make_key()); }
    void set_occupiable_fields(const std::vector<int>& fns);
    void set_occupiable_board_fields(const int field_number);
    void set_occupiable_empty_board_fields();
    void set_occupiable_empty_fields();
    void change_engine_active(Player* player, const bool is_active);
    void enter_setup_mode();
    void leave_setup_mode();
    void check_hide_drawer();
    void check_show_prison();
    boardgame::Diff diff_key(Muehle_Key key);
    const boardgame::Piecegroup<decltype(current_constellation.cbegin())> white_pieces { make_piecegroup(current_constellation, muehle::first_white_piece, muehle::number_of_pieces_per_player) };
    const boardgame::Piecegroup<decltype(current_constellation.cbegin())> black_pieces { make_piecegroup(current_constellation, muehle::first_black_piece, muehle::number_of_pieces_per_player) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> game_board { make_fieldgroup(all_fields, muehle::first_board_field, muehle::number_of_board_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> white_drawer { make_fieldgroup(all_fields, muehle::first_white_drawer_field, muehle::number_of_drawer_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> black_drawer { make_fieldgroup(all_fields, muehle::first_black_drawer_field, muehle::number_of_drawer_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> white_prison { make_fieldgroup(all_fields, muehle::first_white_prison_field, muehle::number_of_prison_fields) };
    const boardgame::Fieldgroup<decltype(all_fields.cbegin())> black_prison { make_fieldgroup(all_fields, muehle::first_black_prison_field, muehle::number_of_prison_fields) };
    Player white_player {white_pieces, white_drawer, white_prison, "white", false};
    Player black_player {black_pieces, black_drawer, black_prison, "black", false};
    const Player* current_player {&white_player};
    const Player* opponent_player {&black_player};
    boardgame::Piece_Number selected_piece {};
    std::vector<boardgame::Piece_Number> selectable_pieces {};
    std::vector<boardgame::Field_Number> m_occupiable_fields {};
    bool setup_mode {false};
    boardgame::Alpha_Beta<muehle::Muehle_Key, muehle::Engine_Helper> engine;
    bool restart {false};
};

}

#endif // SRC_MUEHLE_MUEHLE_STATE
