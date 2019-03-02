#ifndef TESTS_MUEHLE_MUEHLE_STATE_TEST
#define TESTS_MUEHLE_MUEHLE_STATE_TEST

#include "boardgame/pieces_n_fields.h"
#include "muehle/muehle_state.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>

class Muehle_Ui_Mock
{
public:
    MOCK_METHOD0(draw, void());
    MOCK_METHOD1(win, void(const std::string&));
    MOCK_METHOD2(drawer_can_hide, void(const std::string&, bool));
    MOCK_METHOD2(prison_can_hide, void(const std::string&, bool));
    MOCK_METHOD1(lock_piece, void(int));
    MOCK_METHOD1(selectable, void(int));
    MOCK_METHOD1(removable, void(int));
    MOCK_METHOD1(lock_field, void(int));
    MOCK_METHOD1(highlight, void(int));
    MOCK_METHOD2(occupiable, void(int, int));
    MOCK_METHOD2(set_field, void(int, int));
    MOCK_METHOD1(setup_mode_active, void(bool));
    MOCK_METHOD2(engine_active, void(const std::string&, bool));
    MOCK_METHOD1(need_confirm, void(bool));
};

class Move_List_Ui_Mock
{
public:
    MOCK_METHOD1(initial_constellation, void(const int));
    MOCK_METHOD1(current_move, void(const int));
    MOCK_METHOD4(add_move, void(const int, const int, const std::string&, const std::vector<int>&));
    MOCK_METHOD1(delete_move, void(const int));
    MOCK_METHOD1(cut_off, void(const int));
    MOCK_METHOD1(need_confirm, void(bool));
};

class Main_Loop_Mock
{
public:
    MOCK_METHOD1(engine_future, void(std::future<bool>&& efu));
};

class Muehle_State_Test
        : public ::testing::Test
{
protected:
    Muehle_State_Test();
    ~Muehle_State_Test() = default;
    void check_new_game();
    void check_all_free_become_locked();
    void check_all_free_become_occupiable(boardgame::Piece_Number pn, boardgame::Field_Number except = boardgame::Field_Number{INT_MAX});
    void phase1_place_piece(boardgame::Piece_Number pn, boardgame::Field_Number to, bool closes_muehle = false);
    ::testing::StrictMock<Muehle_Ui_Mock> mUi {};
    ::testing::StrictMock<Move_List_Ui_Mock> mlUi {};
    ::testing::StrictMock<Main_Loop_Mock> ml {};
    muehle::Muehle_State mM {boardgame::Boardgame_Ui(&mUi), boardgame::Move_List_Ui(&mlUi), boardgame::Main_Loop(&ml)};
    int mNextInWhiteDrawer {0};
    int mNextInBlackDrawer {0};
    std::vector<int> mOccupiedBoardFields {};
};

void draw(::testing::StrictMock<Muehle_Ui_Mock>* mui) { mui->draw(); }
void win(::testing::StrictMock<Muehle_Ui_Mock>* mui, const std::string& player) { mui->win(player); }
void initial_constellation(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int constellation_id) { mui->initial_constellation(constellation_id); }
void current_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->current_move(move_id); }
void add_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id, const int branch_id, const std::string& description, const std::vector<int>& hint) { mui->add_move(move_id, branch_id, description, hint); }
void delete_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->delete_move(move_id); }
void cut_off(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->cut_off(move_id); }
void need_confirm(::testing::StrictMock<Move_List_Ui_Mock>* mui, const bool in) { mui->need_confirm(in); }
void drawer_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, const std::string& player_id, bool can_hide) { mui->drawer_can_hide(player_id, can_hide); }
void prison_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, const std::string& player_id, bool can_hide) { mui->prison_can_hide(player_id, can_hide); }
void lock_piece(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Piece_Number piece_id) { mui->lock_piece(piece_id.v); }
void selectable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Piece_Number piece_id) { mui->selectable(piece_id.v); }
void removable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Piece_Number piece_id) { mui->removable(piece_id.v); }
void lock_field(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Field_Number field_id) {  mui->lock_field(field_id.v); }
void highlight(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Field_Number field_id) { mui->highlight(field_id.v); }
void occupiable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Field_Number field_id, boardgame::Piece_Number piece_id) { mui->occupiable(field_id.v, piece_id.v); }
void set_field(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::Piece_Number piece_id, boardgame::Field_Number field_id) { mui->set_field(piece_id.v, field_id.v); }
void setup_mode_active(::testing::StrictMock<Muehle_Ui_Mock>* mui, const bool is_active) { mui->setup_mode_active(is_active); }
void engine_active(::testing::StrictMock<Muehle_Ui_Mock>* mui, const std::string& player_id, const bool is_active) { mui->engine_active(player_id, is_active); }
void active_player(::testing::StrictMock<Muehle_Ui_Mock>* mui, const std::string& player_id) { (void)mui; (void)player_id; }
void engine_future(::testing::StrictMock<Main_Loop_Mock>* ml, std::future<bool>&& efu) { ml->engine_future(std::move(efu)); }
void need_confirm(::testing::StrictMock<Muehle_Ui_Mock>* mui, const bool in) { mui->need_confirm(in); }

# endif // TESTS_MUEHLE_MUEHLE_STATE_TEST
