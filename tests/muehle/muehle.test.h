#ifndef TESTS_MUEHLE_MUEHLE_TEST
#define TESTS_MUEHLE_MUEHLE_TEST

#include "boardgame/pieces_n_fields.h"
#include "muehle/muehle.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>

class Muehle_Ui_Mock
{
public:
    MOCK_METHOD0(draw, void());
    MOCK_METHOD1(win, void(const std::string&));
    MOCK_METHOD1(white_drawer_can_hide, void(bool));
    MOCK_METHOD1(black_drawer_can_hide, void(bool));
    MOCK_METHOD1(white_prison_can_hide, void(bool));
    MOCK_METHOD1(black_prison_can_hide, void(bool));
    MOCK_METHOD0(lock_pieces, void());
    MOCK_METHOD1(selectable, void(int));
    MOCK_METHOD1(removable, void(int));
    MOCK_METHOD1(lock_field, void(int));
    MOCK_METHOD1(highlight, void(int));
    MOCK_METHOD2(occupiable, void(int, int));
    MOCK_METHOD2(set_field, void(int, int));
};

class Move_List_Ui_Mock
{
public:
    MOCK_METHOD1(initial_constellation, void(const int));
    MOCK_METHOD1(current_move, void(const int));
    MOCK_METHOD4(add_move, void(const int, const int, const std::string&, const std::vector<int>&));
    MOCK_METHOD1(delete_move, void(const int));
    MOCK_METHOD1(cut_off, void(const int));
};

class Muehle_Test
        : public ::testing::Test
{
protected:
    Muehle_Test();
    ~Muehle_Test() = default;
    void checkNewGame();
    void checkAllFieldsBecomeLocked();
    void checkAllFreeBecomeOccupiable(boardgame::PieceNumber pn, boardgame::FieldNumber except = boardgame::FieldNumber{INT_MAX});
    void phase1PlacePiece(boardgame::PieceNumber pn, boardgame::FieldNumber to, bool closesMuehle = false);
    ::testing::StrictMock<Muehle_Ui_Mock> mUi {};
    ::testing::StrictMock<Move_List_Ui_Mock> mlUi {};
    Muehle mM {Muehle_Ui_Type(&mUi), Move_List_Ui(&mlUi)};
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
void white_drawer_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, bool can_hide) { mui->white_drawer_can_hide(can_hide); }
void black_drawer_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, bool can_hide) { mui->black_drawer_can_hide(can_hide); }
void white_prison_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, bool can_hide) { mui->white_prison_can_hide(can_hide); }
void black_prison_can_hide(::testing::StrictMock<Muehle_Ui_Mock>* mui, bool can_hide) { mui->black_prison_can_hide(can_hide); }
void lock_pieces(::testing::StrictMock<Muehle_Ui_Mock>* mui) { mui->lock_pieces(); }
void selectable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::PieceNumber piece_id) { mui->selectable(piece_id.v); }
void removable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::PieceNumber piece_id) { mui->removable(piece_id.v); }
void lock_field(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::FieldNumber field_id) {  mui->lock_field(field_id.v); }
void highlight(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::FieldNumber field_id) { mui->highlight(field_id.v); }
void occupiable(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::FieldNumber field_id, boardgame::PieceNumber piece_id) { mui->occupiable(field_id.v, piece_id.v); }
void set_field(::testing::StrictMock<Muehle_Ui_Mock>* mui, boardgame::PieceNumber piece_id, boardgame::FieldNumber field_id) { mui->set_field(piece_id.v, field_id.v); }

# endif // TESTS_MUEHLE_MUEHLE_TEST
