#include "testmuehle.h"

using namespace testing;
using namespace std;

MuehlePieceViewMock::MuehlePieceViewMock(string color)
    : MuehlePieceView(color)
{
}

TestMuehle::TestMuehle()
    : Test()
    , mWhitePieceView("whitetest")
    , mBlackPieceView("blacktest")
    , mM(&mControl,
         &mWhitePieceView, &mBlackPieceView, &mWhiteDrawerView, &mBlackDrawerView,
         &mWhitePrisonView, &mBlackPrisonView, &mBoardView)
{
}

void TestMuehle::checkNewGame()
{
    for (int i = 0; i < 9; i += 1) {
        EXPECT_CALL(mWhitePieceView, normalState(i));
        EXPECT_CALL(mWhitePieceView, setField(MuehleFieldId{Muehle::whitedrawer(), i}, i));
        EXPECT_CALL(mBlackPieceView, normalState(i));
        EXPECT_CALL(mBlackPieceView, setField(MuehleFieldId{Muehle::blackdrawer(), i}, i));
        EXPECT_CALL(mWhiteDrawerView, setPiece(MuehlePieceId{Muehle::white(), i}, i));
        EXPECT_CALL(mBlackDrawerView, setPiece(MuehlePieceId{Muehle::black(), i}, i));
    }
    for (int i = 0; i < 24; i += 1) {
        EXPECT_CALL(mBoardView, normalState(i));
        EXPECT_CALL(mBoardView, destinationState(i, "whitetest"));
    }
    EXPECT_CALL(mWhitePieceView, movingState(0));
}

TEST_F(TestMuehle, testNewGame)
{
    checkNewGame();
    mM.newGame();
}

TEST_F(TestMuehle, testPhase1)
{
    checkNewGame();
    mM.newGame();
    for (int i = 0; i < 5; i += 1) {
        EXPECT_CALL(mBoardView, normalState(i));
        EXPECT_CALL(mBoardView, destinationState(i, "blacktest"));
    }
    EXPECT_CALL(mBoardView, normalState(5)); // field 5 gets occupied and will not be offered as destination
    for (int i = 6; i < 24; i += 1) {
        EXPECT_CALL(mBoardView, normalState(i));
        EXPECT_CALL(mBoardView, destinationState(i, "blacktest"));
    }
    for (int i = 0; i < 9; i += 1) {
        EXPECT_CALL(mWhitePieceView, normalState(i));
        EXPECT_CALL(mBlackPieceView, normalState(i));
    }
    EXPECT_CALL(mWhiteDrawerView, setPiece(MuehlePieceId{"", -1}, 0));
    EXPECT_CALL(mBoardView, setPiece(MuehlePieceId{Muehle::white(), 0}, 5));
    EXPECT_CALL(mWhitePieceView, setField(MuehleFieldId{Muehle::board(), 5}, 0));
    EXPECT_CALL(mBlackPieceView, movingState(0));
    mBoardView.destinationSelected(5);
}
