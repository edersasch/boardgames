#include "muehle.h"
#include "muehlepieceview.h"
#include "muehlefieldview.h"
#include "muehlecontrol.h"
#include "muehlepiecemodel.h"
#include "muehlefieldmodel.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>

class MuehlePieceViewMock
        : public MuehlePieceView
{
public:
    MuehlePieceViewMock(std::string color);
    ~MuehlePieceViewMock() = default;
    MOCK_METHOD2(setField, void(MuehleFieldId, std::size_t pos));
    MOCK_METHOD1(normalState, void(std::size_t pos));
    MOCK_METHOD1(canMoveState, void(std::size_t pos));
    MOCK_METHOD1(movingState, void(std::size_t pos));
};

class MuehleFieldViewMock
        : public MuehleFieldView
{
public:
    MOCK_METHOD2(setPiece, void(MuehlePieceId, std::size_t pos));
    MOCK_METHOD1(normalState, void(std::size_t pos));
    MOCK_METHOD2(destinationState, void(std::size_t pos, const std::string& color));
};

class MuehleControlMock
        : public MuehleControl
{
public:
    MOCK_METHOD0(draw, void());
    MOCK_METHOD1(win, void(std::string));
};

class TestMuehle
        : public ::testing::Test
{
protected:
    TestMuehle();
    virtual ~TestMuehle() = default;
    ::testing::StrictMock<MuehlePieceViewMock> mWhitePieceView;
    ::testing::StrictMock<MuehlePieceViewMock> mBlackPieceView;
    ::testing::StrictMock<MuehleFieldViewMock> mWhiteDrawerView {};
    ::testing::StrictMock<MuehleFieldViewMock> mBlackDrawerView {};
    ::testing::StrictMock<MuehleFieldViewMock> mWhitePrisonView {};
    ::testing::StrictMock<MuehleFieldViewMock> mBlackPrisonView {};
    ::testing::StrictMock<MuehleFieldViewMock> mBoardView {};
    ::testing::StrictMock<MuehleControlMock> mControl {};
    Muehle mM;
    void checkNewGame();
};
