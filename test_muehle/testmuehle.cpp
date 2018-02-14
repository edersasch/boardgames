#include "testmuehle.h"
#include "muehleelements.h"

using namespace testing;
using namespace std;

MuehleFieldUiMock::MuehleFieldUiMock(MuehleField* field)
    : MuehleFieldUi(field)
{
}

MuehlePieceUiMock::MuehlePieceUiMock(MuehlePiece* piece)
    : MuehlePieceUi(piece)
{
}

TestMuehle::TestMuehle()
    : Test()
{
    MuehleElements* elem = mM.muehleElements();
    EXPECT_EQ(9, Muehle::numberOfPieces());
    EXPECT_EQ(24, Muehle::numberOfFields());
    EXPECT_EQ(Muehle::numberOfPieces(), elem->pieceGroup(Muehle::white())->size());
    EXPECT_EQ(Muehle::numberOfPieces(), elem->pieceGroup(Muehle::black())->size());
    EXPECT_EQ(Muehle::numberOfPieces(), elem->fieldGroup(Muehle::whitedrawer())->size());
    EXPECT_EQ(Muehle::numberOfPieces(), elem->fieldGroup(Muehle::blackdrawer())->size());
    EXPECT_EQ(Muehle::numberOfPieces() - 2, elem->fieldGroup(Muehle::whiteprison())->size());
    EXPECT_EQ(Muehle::numberOfPieces() - 2, elem->fieldGroup(Muehle::blackprison())->size());
    EXPECT_EQ(Muehle::numberOfFields(), elem->fieldGroup(Muehle::board())->size());
    for (std::size_t i = 0; i < Muehle::numberOfPieces(); i += 1) {
        mWhitePieceUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehlePieceUiMock>>(elem->pieceGroup(Muehle::white())->at(i)));
        mBlackPieceUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehlePieceUiMock>>(elem->pieceGroup(Muehle::black())->at(i)));
        mWhiteDrawerFieldUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehleFieldUiMock>>(elem->fieldGroup(Muehle::whitedrawer())->at(i)));
        mBlackDrawerFieldUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehleFieldUiMock>>(elem->fieldGroup(Muehle::blackdrawer())->at(i)));
    }
    for (std::size_t i = 0; i < Muehle::numberOfPieces() - 2; i += 1) {
        mWhitePrisonFieldUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehleFieldUiMock>>(elem->fieldGroup(Muehle::whiteprison())->at(i)));
        mBlackPrisonFieldUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehleFieldUiMock>>(elem->fieldGroup(Muehle::blackprison())->at(i)));
    }
    elem->pieceGroup(Muehle::white())->setColor(whiteTest());
    elem->pieceGroup(Muehle::black())->setColor(blackTest());
    for (std::size_t i = 0; i < Muehle::numberOfFields(); i += 1) {
        mBoardFieldUiMocks.emplace_back(make_unique<::testing::StrictMock<MuehleFieldUiMock>>(elem->fieldGroup(Muehle::board())->at(i)));
    }
}

void TestMuehle::checkNewGame()
{
    for (std::size_t i = 0; i < Muehle::numberOfPieces(); i += 1) {
        EXPECT_CALL(*mWhitePieceUiMocks.at(i).get(), pin());
        EXPECT_CALL(*mWhitePieceUiMocks.at(i).get(), fieldChanged(mM.muehleElements()->fieldGroup(Muehle::whitedrawer())->at(i)->fieldUi()));
        EXPECT_CALL(*mBlackPieceUiMocks.at(i).get(), pin());
        EXPECT_CALL(*mBlackPieceUiMocks.at(i).get(), fieldChanged(mM.muehleElements()->fieldGroup(Muehle::blackdrawer())->at(i)->fieldUi()));
        EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(i).get(), lock());
        EXPECT_CALL(*mBlackDrawerFieldUiMocks.at(i).get(), lock());
    }
    for (std::size_t i = 0; i < Muehle::numberOfPieces() - 2; i += 1) {
        EXPECT_CALL(*mWhitePrisonFieldUiMocks.at(i).get(), lock());
        EXPECT_CALL(*mBlackPrisonFieldUiMocks.at(i).get(), lock());
    }
    checkAllFreeBecomeOccupiable(whiteTest());
    EXPECT_CALL(*mWhitePieceUiMocks.at(0).get(), selectable());
    EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(0).get(), highlight());
}

void TestMuehle::checkAllFieldsBecomeLocked()
{
    for (std::size_t i = 0; i < Muehle::numberOfFields(); i += 1) {
        EXPECT_CALL(*mBoardFieldUiMocks.at(i).get(), lock());
    }
}

void TestMuehle::checkAllFreeBecomeOccupiable(std::string color, size_t except)
{
    for (std::size_t i = 0; i < Muehle::numberOfFields(); i += 1) {
        EXPECT_CALL(*mBoardFieldUiMocks.at(i).get(), lock());
        if (!mM.muehleElements()->fieldGroup(Muehle::board())->at(i)->piece() && i != except) {
            EXPECT_CALL(*mBoardFieldUiMocks.at(i).get(), occupiable(color));
        }
    }
}

void TestMuehle::phase1PlacePiece(std::string color, std::size_t to, bool closesMuehle)
{
    if (color == whiteTest()) {
        if (closesMuehle) {
            checkAllFieldsBecomeLocked();
        } else {
            checkAllFreeBecomeOccupiable(blackTest(), to);
            EXPECT_CALL(*mBlackPieceUiMocks.at(mNextInBlackDrawer).get(), selectable());
            EXPECT_CALL(*mBlackDrawerFieldUiMocks.at(mNextInBlackDrawer).get(), highlight());
        }
        EXPECT_CALL(*mWhitePieceUiMocks.at(mNextInWhiteDrawer).get(), fieldChanged(mM.muehleElements()->fieldGroup(Muehle::board())->at(to)->fieldUi()));
        EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(mNextInWhiteDrawer).get(), lock());
        mNextInWhiteDrawer += 1;
    } else {
        if (closesMuehle) {
            checkAllFieldsBecomeLocked();
        } else {
            checkAllFreeBecomeOccupiable(whiteTest(), to);
            EXPECT_CALL(*mWhitePieceUiMocks.at(mNextInWhiteDrawer).get(), selectable());
            EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(mNextInWhiteDrawer).get(), highlight());
        }
        EXPECT_CALL(*mBlackPieceUiMocks.at(mNextInBlackDrawer).get(), fieldChanged(mM.muehleElements()->fieldGroup(Muehle::board())->at(to)->fieldUi()));
        EXPECT_CALL(*mBlackDrawerFieldUiMocks.at(mNextInBlackDrawer).get(), lock());
        mNextInBlackDrawer += 1;
    }
    for (std::size_t i = 0; i < mNextInWhiteDrawer; i += 1) {
        EXPECT_CALL(*mWhitePieceUiMocks.at(i).get(), pin());
    }
    for (std::size_t i = 0; i < mNextInBlackDrawer; i += 1) {
        EXPECT_CALL(*mBlackPieceUiMocks.at(i).get(), pin());
    }
    mM.muehleElements()->fieldGroup(Muehle::board())->at(to)->occupy();
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
    phase1PlacePiece(whiteTest(), 5);
    phase1PlacePiece(blackTest(), 9);
    phase1PlacePiece(whiteTest(), 13);
    phase1PlacePiece(blackTest(), 10);
    EXPECT_CALL(*mBlackPieceUiMocks.at(0).get(), removable());
    EXPECT_CALL(*mBlackPieceUiMocks.at(1).get(), removable());
    phase1PlacePiece(whiteTest(), 20, true);
}

TEST_F(TestMuehle, testEnterLeaveSetupMode)
{
    checkNewGame();
    mM.newGame();
    for (std::size_t i = 0; i < Muehle::numberOfPieces(); i += 1) {
        EXPECT_CALL(*mWhitePieceUiMocks.at(i).get(), selectable());
        EXPECT_CALL(*mBlackPieceUiMocks.at(i).get(), selectable());
        EXPECT_CALL(*mBlackDrawerFieldUiMocks.at(i).get(), lock()).Times(2);
    }
    for (std::size_t i = 0; i < Muehle::numberOfPieces() - 2; i += 1) {
        EXPECT_CALL(*mWhitePrisonFieldUiMocks.at(i).get(), occupiable(whiteTest()));
        EXPECT_CALL(*mBlackPrisonFieldUiMocks.at(i).get(), lock()).Times(2);
    }
    for (std::size_t i = 0; i < Muehle::numberOfFields(); i += 1) {
        EXPECT_CALL(*mBoardFieldUiMocks.at(i).get(), occupiable(whiteTest())).Times(2);
    }
    mM.enterSetupMode();
    mM.muehleElements()->pieceGroup(Muehle::white())->at(0)->selected();
    for (std::size_t i = 1; i < Muehle::numberOfPieces() - 2; i += 1) {
        EXPECT_CALL(*mWhitePrisonFieldUiMocks.at(i).get(), occupiable(whiteTest()));
    }
    EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(0).get(), occupiable(whiteTest()));
    EXPECT_CALL(*mWhiteDrawerFieldUiMocks.at(0).get(), lock());
    EXPECT_CALL(*mWhitePieceUiMocks.at(0).get(), fieldChanged(mM.muehleElements()->fieldGroup(Muehle::whiteprison())->at(0)->fieldUi()));
    mM.muehleElements()->fieldGroup(Muehle::whiteprison())->at(0)->occupy();

}
