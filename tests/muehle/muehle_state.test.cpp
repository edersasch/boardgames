#include "muehle_state.test.h"

Muehle_State_Test::Muehle_State_Test()
{
    constexpr int zero       =  0; // firstWhitePiece, firstBoardField
    constexpr int nine       =  9; // firstBlackPiece, numberOfPiecesPerPlayer, numberOfDrawerFields
    constexpr int twentyfour = 24; // numberOfBoardFields, firstWhiteDrawerField
    static_assert(boardgame::Piece_Range{nine}        == muehle::number_of_pieces_per_player);
    static_assert(boardgame::Piece_Range{18}          == muehle::number_of_pieces);
    static_assert(boardgame::Piece_Number{zero}       == muehle::first_white_piece);
    static_assert(boardgame::Piece_Number{nine}       == muehle::first_black_piece);
    static_assert(boardgame::Field_Range{twentyfour}  == muehle::number_of_board_fields);
    static_assert(boardgame::Field_Range{nine}        == muehle::number_of_drawer_fields);
    static_assert(boardgame::Field_Range{7}           == muehle::number_of_prison_fields);
    static_assert(boardgame::Field_Range{56}          == muehle::number_of_fields);
    static_assert(boardgame::Field_Number{zero}       == muehle::first_board_field);
    static_assert(boardgame::Field_Number{twentyfour} == muehle::first_white_drawer_field);
    static_assert(boardgame::Field_Number{33}         == muehle::first_black_drawer_field);
    static_assert(boardgame::Field_Number{42}         == muehle::first_white_prison_field);
    static_assert(boardgame::Field_Number{49}         == muehle::first_black_prison_field);
}

void Muehle_State_Test::check_new_game()
{
    for (int i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
        EXPECT_CALL(mUi, set_field(muehle::first_white_piece.v + i, muehle::first_white_drawer_field.v + i));
        EXPECT_CALL(mUi, set_field(muehle::first_black_piece.v + i, muehle::first_black_drawer_field.v + i));
    }
    EXPECT_CALL(mlUi, initial_constellation(0));
    EXPECT_CALL(mlUi, current_move(0));
    check_all_free_become_occupiable(boardgame::Piece_Number{0});
    EXPECT_CALL(mUi, selectable(0));
    EXPECT_CALL(mUi, highlight(muehle::first_white_drawer_field.v));
    EXPECT_CALL(mUi, drawer_can_hide(muehle::white_id, false));
    EXPECT_CALL(mUi, drawer_can_hide(muehle::black_id, false));
    EXPECT_CALL(mUi, prison_can_hide(muehle::white_id, true));
    EXPECT_CALL(mUi, prison_can_hide(muehle::black_id, true));
    EXPECT_CALL(mUi, setup_mode_active(false));
    EXPECT_CALL(mUi, engine_active(muehle::white_id, false));
    EXPECT_CALL(mUi, need_confirm(false));
    EXPECT_CALL(mUi, movecount(0));
    EXPECT_CALL(mUi, time_accounting_correct(true));
    EXPECT_CALL(mUi, player_time(muehle::white_id, std::chrono::milliseconds(0)));
    EXPECT_CALL(mUi, player_time(muehle::black_id, std::chrono::milliseconds(0)));
}

void Muehle_State_Test::check_all_free_become_locked()
{
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        if (std::find(mOccupiedBoardFields.begin(), mOccupiedBoardFields.end(), i) == mOccupiedBoardFields.end()) {
            EXPECT_CALL(mUi, lock_field(muehle::first_board_field.v + i));
        }
    }
}

void Muehle_State_Test::check_all_free_become_occupiable(boardgame::Piece_Number pn, boardgame::Field_Number except)
{
    mOccupiedBoardFields.push_back(except.v);
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        if (std::find(mOccupiedBoardFields.begin(), mOccupiedBoardFields.end(), i) == mOccupiedBoardFields.end()) {
            EXPECT_CALL(mUi, occupiable(muehle::first_board_field.v + i, pn.v));
        }
    }
}

void Muehle_State_Test::phase1_place_piece(boardgame::Piece_Number pn, boardgame::Field_Number to, bool closes_muehle, int count)
{
    check_all_free_become_locked();
    if (pn.v < 9) {
        if (!closes_muehle) {
            check_all_free_become_occupiable(boardgame::Piece_Number{muehle::first_black_piece.v + mNextInBlackDrawer}, to);
            EXPECT_CALL(mUi, selectable(mNextInBlackDrawer + muehle::first_black_piece.v));
            EXPECT_CALL(mUi, highlight(muehle::first_black_drawer_field.v + mNextInBlackDrawer));
            EXPECT_CALL(mlUi, add_move(testing::_, testing::_, testing::_));
            EXPECT_CALL(mlUi, current_move(testing::_));
            EXPECT_CALL(mUi, movecount(count));
        }
        EXPECT_CALL(mUi, set_field(muehle::first_white_piece.v + mNextInWhiteDrawer, muehle::first_board_field.v + to.v));
        mNextInWhiteDrawer += 1;
    } else {
        if (!closes_muehle) {
            check_all_free_become_occupiable(boardgame::Piece_Number{muehle::first_white_piece.v + mNextInWhiteDrawer}, to);
            EXPECT_CALL(mUi, selectable(mNextInWhiteDrawer + muehle::first_white_piece.v));
            EXPECT_CALL(mUi, highlight(muehle::first_white_drawer_field.v + mNextInWhiteDrawer));
            EXPECT_CALL(mlUi, add_move(testing::_, testing::_, testing::_));
            EXPECT_CALL(mlUi, current_move(testing::_));
            EXPECT_CALL(mUi, movecount(count));
        }
        EXPECT_CALL(mUi, set_field(muehle::first_black_piece.v + mNextInBlackDrawer, muehle::first_board_field.v + to.v));
        mNextInBlackDrawer += 1;
    }
    EXPECT_CALL(mUi, lock_piece(pn.v));
    mM.request_occupy(to);
}

TEST_F(Muehle_State_Test, testNewGame)
{
    check_new_game();
    mM.new_game();
}

TEST_F(Muehle_State_Test, testPhase1)
{
    check_new_game();
    mM.new_game();
    EXPECT_CALL(mUi, lock_field(24));
    EXPECT_CALL(mUi, need_confirm(true));
    EXPECT_CALL(mlUi, need_confirm(true));
    phase1_place_piece(boardgame::Piece_Number{0}, boardgame::Field_Number{5}, false, 1);
    EXPECT_CALL(mUi, lock_field(33));
    phase1_place_piece(boardgame::Piece_Number{9}, boardgame::Field_Number{9}, false, 1);
    EXPECT_CALL(mUi, lock_field(25));
    phase1_place_piece(boardgame::Piece_Number{1}, boardgame::Field_Number{13}, false, 2);
    EXPECT_CALL(mUi, lock_field(34));
    phase1_place_piece(boardgame::Piece_Number{10}, boardgame::Field_Number{10}, false, 2);
    EXPECT_CALL(mUi, removable(9));
    EXPECT_CALL(mUi, removable(10));
    EXPECT_CALL(mUi, lock_field(26));
    phase1_place_piece(boardgame::Piece_Number{2}, boardgame::Field_Number{20}, true, 3);
}

TEST_F(Muehle_State_Test, testEnterLeaveSetupMode)
{
    check_new_game();
    mM.new_game();
    for (int i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
        EXPECT_CALL(mUi, selectable(i + muehle::first_white_piece.v));
        EXPECT_CALL(mUi, selectable(i + muehle::first_black_piece.v));
    }
    for (int i = 0; i < muehle::number_of_pieces_per_player.v - 2; i += 1) {
        EXPECT_CALL(mUi, occupiable(muehle::first_white_prison_field.v + i, 0));
    }
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        EXPECT_CALL(mUi, occupiable(muehle::first_board_field.v + i, 0));
    }
    EXPECT_CALL(mUi, drawer_can_hide(muehle::white_id, false));
    EXPECT_CALL(mUi, drawer_can_hide(muehle::black_id, false));
    EXPECT_CALL(mUi, prison_can_hide(muehle::white_id, false));
    EXPECT_CALL(mUi, prison_can_hide(muehle::black_id, false));
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(muehle::first_board_field.v + i));
    }
    EXPECT_CALL(mUi, setup_mode_active(true));
    EXPECT_CALL(mUi, engine_active(muehle::white_id, false));
    EXPECT_CALL(mUi, engine_active(muehle::black_id, false));
    EXPECT_CALL(mlUi, initial_constellation(0));
    EXPECT_CALL(mlUi, current_move(0));
    EXPECT_CALL(mlUi, need_confirm(true));
    EXPECT_CALL(mUi, time_accounting_correct(false));
    mM.request_setup_mode_active(true);
    mM.request_select_piece(boardgame::Piece_Number{0});
    for (int i = 1; i < muehle::number_of_pieces_per_player.v - 2; i += 1) {
        EXPECT_CALL(mUi, occupiable(muehle::first_white_prison_field.v + i, 0));
    }
    EXPECT_CALL(mUi, occupiable(muehle::first_white_drawer_field.v, 0));
    EXPECT_CALL(mUi, set_field(muehle::first_white_piece.v, muehle::first_white_prison_field.v));
    EXPECT_CALL(mUi, highlight(muehle::first_white_prison_field.v));
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(muehle::first_board_field.v + i));
    }
    for (int i = 0; i < muehle::number_of_pieces_per_player.v - 2; i += 1) {
        EXPECT_CALL(mUi, lock_field(muehle::first_white_prison_field.v + i));
    }
    for (int i = 0; i < muehle::number_of_board_fields.v; i += 1) {
        EXPECT_CALL(mUi, occupiable(muehle::first_board_field.v + i, 0));
    }
    EXPECT_CALL(mUi, lock_field(muehle::first_white_drawer_field.v));
    EXPECT_CALL(mUi, lock_piece(muehle::first_white_piece.v));
    EXPECT_CALL(mUi, selectable(muehle::first_white_piece.v));
    mM.request_occupy(boardgame::Field_Number{muehle::first_white_prison_field});
}
