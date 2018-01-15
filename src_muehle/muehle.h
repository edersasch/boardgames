#ifndef SRC_MUEHLE_MUEHLE
#define SRC_MUEHLE_MUEHLE

#include "muehlefieldmodel.h"
#include "muehlepiecemodel.h"

#include <unordered_map>

class MuehlePieceView;
class MuehleFieldView;
class MuehleControl;

/*
   0--------1--------2
   |        |        |
   |  3-----4-----5  |
   |  |     |     |  |
   |  |  6--7--8  |  |
   9-10-11    12-13-14
   |  | 15-16-17  |  |
   |  |     |     |  |
   |  18---19----20  |
   |        |        |
   21------22-------23
*/

class Muehle
{
public:
    Muehle(MuehleControl* control,
           MuehlePieceView* whitePieceView, MuehlePieceView* blackPieceView,
           MuehleFieldView* whiteDrawerView, MuehleFieldView* blackDrawerView,
           MuehleFieldView* whitePrisonView, MuehleFieldView* blackPrisonView,
           MuehleFieldView* boardView);
    ~Muehle() = default;
    static constexpr int NUMBER_OF_PIECES {9};
    static constexpr int NUMBER_OF_FIELDS {24};
    static constexpr const char* white() { return "white"; }
    static constexpr const char* black() { return "black"; }
    static constexpr const char* board() { return "board"; }
    static constexpr const char* whitedrawer() { return "whitedrawer"; }
    static constexpr const char* blackdrawer() { return "blackdrawer"; }
    static constexpr const char* whiteprison() { return "whiteprison"; }
    static constexpr const char* blackprison() { return "blackprison"; }
    void newGame();
    void destinationSelected(int fieldNumber);
private:
    struct Player
    {
        MuehlePieceModel& mPieces;
        MuehleFieldModel& mDrawer;
        MuehleFieldModel& mPrison;
    };
    void startMove();
    void movePiece(MuehlePieceId p, MuehleFieldId f);
    void setCurrentPlayer(Player* pl);
    MuehleControl* mControl;
    std::unordered_map<std::string, MuehlePieceModel> mPieces {
        {white(), MuehlePieceModel(NUMBER_OF_PIECES)},
        {black(), MuehlePieceModel(NUMBER_OF_PIECES)}
    };
    std::unordered_map<std::string, MuehleFieldModel> mFields {
        {board(), MuehleFieldModel(NUMBER_OF_FIELDS)},
        {whitedrawer(), MuehleFieldModel(NUMBER_OF_PIECES)},
        {blackdrawer(), MuehleFieldModel(NUMBER_OF_PIECES)},
        {whiteprison(), MuehleFieldModel(NUMBER_OF_PIECES - 2)},
        {blackprison(), MuehleFieldModel(NUMBER_OF_PIECES - 2)}
    };
    Player mWhite {mPieces.at(white()), mFields.at(whitedrawer()), mFields.at(whiteprison())};
    Player mBlack {mPieces.at(black()), mFields.at(blackdrawer()), mFields.at(blackprison())};
    Player* mCurrent {&mWhite};
    Player* mOpponent {&mBlack};
    MuehlePieceId mSelectedPiece {"", -1};
    bool mOnlyOnePieceSelectable {false};
};

#endif // SRC_MUEHLE_MUEHLE
