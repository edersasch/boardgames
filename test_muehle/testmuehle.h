#include "muehle.h"
#include "muehleui.h"
#include "muehlefieldui.h"
#include "muehlepieceui.h"
#include "muehleelements.h"
#include "muehlepiecegroup.h"
#include "muehlefieldgroup.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>

class MuehleGameUiMock
        : public MuehleUi
{
public:
    MOCK_METHOD0(draw, void());
    MOCK_METHOD1(win, void(const std::string&));
};

class MuehleFieldUiMock
        : public MuehleFieldUi
{
public:
    MuehleFieldUiMock(MuehleField* field);
    MOCK_METHOD0(lock, void());
    MOCK_METHOD1(occupiable, void(const std::string&));
};

class MuehlePieceUiMock
        : public MuehlePieceUi
{
public:
    MuehlePieceUiMock(MuehlePiece* piece);
    MOCK_METHOD1(fieldChanged, void(MuehleFieldUi*));
    MOCK_METHOD0(pin, void());
    MOCK_METHOD0(selectable, void());
    MOCK_METHOD0(removable, void());
};

class TestMuehle
        : public ::testing::Test
{
protected:
    TestMuehle();
    virtual ~TestMuehle() = default;
    void checkNewGame();
    void checkAllFieldsBecomeNormal();
    void checkAllFreeBecomeDestination(std::string color, std::size_t except = UINT_MAX);
    void phase1PlacePiece(std::string color, std::size_t to, bool closesMuehle = false);
    static constexpr const char* whiteTest() { return "whitetest"; }
    static constexpr const char* blackTest() { return "blacktest"; }
    ::testing::StrictMock<MuehleGameUiMock> mUi {};
    Muehle mM {&mUi};
    std::vector<std::unique_ptr<::testing::StrictMock<MuehlePieceUiMock>>> mWhitePieceUiMocks;
    std::vector<std::unique_ptr<::testing::StrictMock<MuehlePieceUiMock>>> mBlackPieceUiMocks;
    std::vector<std::unique_ptr<::testing::StrictMock<MuehleFieldUiMock>>> mWhiteDrawerFieldUiMocks;
    std::vector<std::unique_ptr<::testing::StrictMock<MuehleFieldUiMock>>> mBlackDrawerFieldUiMocks;
    std::vector<std::unique_ptr<::testing::StrictMock<MuehleFieldUiMock>>> mBoardFieldUiMocks;
    std::size_t mNextInWhiteDrawer {0};
    std::size_t mNextInBlackDrawer {0};
};
