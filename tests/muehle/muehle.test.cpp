#include "muehle.test.h"

Muehle_Test::Muehle_Test()
{
    constexpr int zero       =  0; // firstWhitePiece, firstBoardField
    constexpr int nine       =  9; // firstBlackPiece, numberOfPiecesPerPlayer, numberOfDrawerFields
    constexpr int twentyfour = 24; // numberOfBoardFields, firstWhiteDrawerField
    static_assert( boardgame::PieceRange{nine}       == Muehle::numberOfPiecesPerPlayer);
    static_assert( boardgame::PieceRange{18}         == Muehle::numberOfPieces);
    static_assert(boardgame::PieceNumber{zero}       == Muehle::firstWhitePiece);
    static_assert(boardgame::PieceNumber{nine}       == Muehle::firstBlackPiece);
    static_assert( boardgame::FieldRange{twentyfour} == Muehle::numberOfBoardFields);
    static_assert( boardgame::FieldRange{nine}       == Muehle::numberOfDrawerFields);
    static_assert( boardgame::FieldRange{7}          == Muehle::numberOfPrisonFields);
    static_assert( boardgame::FieldRange{56}         == Muehle::numberOfFields);
    static_assert(boardgame::FieldNumber{zero}       == Muehle::firstBoardField);
    static_assert(boardgame::FieldNumber{twentyfour} == Muehle::firstWhiteDrawerField);
    static_assert(boardgame::FieldNumber{33}         == Muehle::firstBlackDrawerField);
    static_assert(boardgame::FieldNumber{42}         == Muehle::firstWhitePrisonField);
    static_assert(boardgame::FieldNumber{49}         == Muehle::firstBlackPrisonField);
}

void Muehle_Test::checkNewGame()
{
    EXPECT_CALL(mUi, lock_pieces());
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v; i += 1) {
        EXPECT_CALL(mUi, set_field(Muehle::firstWhitePiece.v + i, Muehle::firstWhiteDrawerField.v + i));
        EXPECT_CALL(mUi, set_field(Muehle::firstBlackPiece.v + i, Muehle::firstBlackDrawerField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhiteDrawerField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackDrawerField.v + i));
    }
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v - 2; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhitePrisonField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackPrisonField.v + i));
    }
    EXPECT_CALL(mlUi, initial_constellation(0));
    EXPECT_CALL(mlUi, current_move(0));
    checkAllFreeBecomeOccupiable(boardgame::PieceNumber{0});
    EXPECT_CALL(mUi, selectable(0));
    EXPECT_CALL(mUi, highlight(Muehle::firstWhiteDrawerField.v));
    EXPECT_CALL(mUi, white_drawer_can_hide(false));
    EXPECT_CALL(mUi, black_drawer_can_hide(false));
    EXPECT_CALL(mUi, white_prison_can_hide(true));
    EXPECT_CALL(mUi, black_prison_can_hide(true));
}

void Muehle_Test::checkAllFieldsBecomeLocked()
{
    for (int i = 0; i < Muehle::numberOfBoardFields.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstBoardField.v + i));
    }
}

void Muehle_Test::checkAllFreeBecomeOccupiable(boardgame::PieceNumber pn, boardgame::FieldNumber except)
{
    mOccupiedBoardFields.push_back(except.v);
    for (int i = 0; i < Muehle::numberOfBoardFields.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstBoardField.v + i));
        if (std::find(mOccupiedBoardFields.begin(), mOccupiedBoardFields.end(), i) == mOccupiedBoardFields.end()) {
            EXPECT_CALL(mUi, occupiable(Muehle::firstBoardField.v + i, pn.v));
        }
    }
}

void Muehle_Test::phase1PlacePiece(boardgame::PieceNumber pn, boardgame::FieldNumber to, bool closesMuehle)
{
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhiteDrawerField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackDrawerField.v + i));
    }
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v - 2; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhitePrisonField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackPrisonField.v + i));
    }
    if (pn.v < 9) {
        if (closesMuehle) {
            checkAllFieldsBecomeLocked();
        } else {
            checkAllFreeBecomeOccupiable(boardgame::PieceNumber{Muehle::firstBlackPiece.v + mNextInBlackDrawer}, to);
            EXPECT_CALL(mUi, selectable(mNextInBlackDrawer + Muehle::firstBlackPiece.v));
            EXPECT_CALL(mUi, highlight(Muehle::firstBlackDrawerField.v + mNextInBlackDrawer));
            EXPECT_CALL(mlUi, add_move(testing::_, testing::_, testing::_, testing::_));
            EXPECT_CALL(mlUi, current_move(testing::_));
        }
        EXPECT_CALL(mUi, set_field(Muehle::firstWhitePiece.v + mNextInWhiteDrawer, Muehle::firstBoardField.v + to.v));
        EXPECT_CALL(mUi, white_drawer_can_hide(false));
        mNextInWhiteDrawer += 1;
    } else {
        if (closesMuehle) {
            checkAllFieldsBecomeLocked();
        } else {
            checkAllFreeBecomeOccupiable(boardgame::PieceNumber{Muehle::firstWhitePiece.v + mNextInWhiteDrawer}, to);
            EXPECT_CALL(mUi, selectable(mNextInWhiteDrawer + Muehle::firstWhitePiece.v));
            EXPECT_CALL(mUi, highlight(Muehle::firstWhiteDrawerField.v + mNextInWhiteDrawer));
            EXPECT_CALL(mlUi, add_move(testing::_, testing::_, testing::_, testing::_));
            EXPECT_CALL(mlUi, current_move(testing::_));
        }
        EXPECT_CALL(mUi, set_field(Muehle::firstBlackPiece.v + mNextInBlackDrawer, Muehle::firstBoardField.v + to.v));
        EXPECT_CALL(mUi, black_drawer_can_hide(false));
        mNextInBlackDrawer += 1;
    }
    EXPECT_CALL(mUi, lock_pieces());
    mM.occupy(to);
}

TEST_F(Muehle_Test, testNewGame)
{
    checkNewGame();
    mM.new_game();
}

TEST_F(Muehle_Test, testPhase1)
{
    checkNewGame();
    mM.new_game();
    phase1PlacePiece(boardgame::PieceNumber{0}, boardgame::FieldNumber{5});
    phase1PlacePiece(boardgame::PieceNumber{9}, boardgame::FieldNumber{9});
    phase1PlacePiece(boardgame::PieceNumber{1}, boardgame::FieldNumber{13});
    phase1PlacePiece(boardgame::PieceNumber{10}, boardgame::FieldNumber{10});
    EXPECT_CALL(mUi, removable(9));
    EXPECT_CALL(mUi, removable(10));
    phase1PlacePiece(boardgame::PieceNumber{2}, boardgame::FieldNumber{20}, true);
}

TEST_F(Muehle_Test, testEnterLeaveSetupMode)
{
    checkNewGame();
    mM.new_game();
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v; i += 1) {
        EXPECT_CALL(mUi, selectable(i + Muehle::firstWhitePiece.v));
        EXPECT_CALL(mUi, selectable(i + Muehle::firstBlackPiece.v));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackDrawerField.v + i));
    }
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v - 2; i += 1) {
        EXPECT_CALL(mUi, occupiable(Muehle::firstWhitePrisonField.v + i, 0));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackPrisonField.v + i));
    }
    for (int i = 0; i < Muehle::numberOfBoardFields.v; i += 1) {
        EXPECT_CALL(mUi, occupiable(Muehle::firstBoardField.v + i, 0)).Times(2);
    }
    EXPECT_CALL(mUi, white_drawer_can_hide(false));
    EXPECT_CALL(mUi, black_drawer_can_hide(false));
    EXPECT_CALL(mUi, white_prison_can_hide(false));
    EXPECT_CALL(mUi, black_prison_can_hide(false));
    mM.enter_setup_mode();
    mM.piece_selected(boardgame::PieceNumber{0});
    for (int i = 1; i < Muehle::numberOfPiecesPerPlayer.v - 2; i += 1) {
        EXPECT_CALL(mUi, occupiable(Muehle::firstWhitePrisonField.v + i, 0));
    }
    EXPECT_CALL(mUi, occupiable(Muehle::firstWhiteDrawerField.v, 0));
    EXPECT_CALL(mUi, set_field(Muehle::firstWhitePiece.v, Muehle::firstWhitePrisonField.v));
    EXPECT_CALL(mUi, highlight(Muehle::firstWhitePrisonField.v));
    for (int i = 0; i < Muehle::numberOfBoardFields.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstBoardField.v + i));
    }
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhiteDrawerField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackDrawerField.v + i)).Times(2);
    }
    for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v - 2; i += 1) {
        EXPECT_CALL(mUi, lock_field(Muehle::firstWhitePrisonField.v + i));
        EXPECT_CALL(mUi, lock_field(Muehle::firstBlackPrisonField.v + i)).Times(2);
    }
    mM.occupy(boardgame::FieldNumber{Muehle::firstWhitePrisonField});
}
