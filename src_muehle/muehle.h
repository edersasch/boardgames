#ifndef SRC_MUEHLE_MUEHLE
#define SRC_MUEHLE_MUEHLE

#include "muehlefieldgroup.h"
#include "muehlepiecegroup.h"
#include "muehleelements.h"

class MuehleUi;

/** Resource setup and rule checking
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
*/

class Muehle
{
public:
    Muehle(MuehleUi* gameUi);
    virtual ~Muehle() = default;
    static constexpr std::size_t numberOfPieces() { return 9; }
    static constexpr std::size_t numberOfFields() { return 24; }
    static constexpr const char* white() { return "white"; }
    static constexpr const char* black() { return "black"; }
    static constexpr const char* board() { return "board"; }
    static constexpr const char* whitedrawer() { return "whitedrawer"; }
    static constexpr const char* blackdrawer() { return "blackdrawer"; }
    static constexpr const char* whiteprison() { return "whiteprison"; }
    static constexpr const char* blackprison() { return "blackprison"; }
    void newGame();
    MuehleElements* muehleElements();
private:
    struct Player
    {
        MuehlePieceGroup* mPieces;
        MuehleFieldGroup* mDrawer;
        MuehleFieldGroup* mPrison;
        int mAlreadyCaptured {0};
    };
    void occupy(MuehleField* field);
    void pieceRemoved(MuehlePiece* removedPiece);
    void pieceSelected(MuehlePiece* selectedPiece);
    void startMove();
    void swapPlayers();
    bool closedMuehle(int fieldNumber);
    std::vector<MuehleField*> freeAdjacentFields(MuehlePiece* p);
    MuehleUi* mUi;
    MuehleElements mElements {
        {
            {white(), numberOfPieces()},
            {black(), numberOfPieces()}
        },
        {
            {board(), numberOfFields()},
            {whitedrawer(), numberOfPieces()},
            {blackdrawer(), numberOfPieces()},
            {whiteprison(), numberOfPieces() - 2},
            {blackprison(), numberOfPieces() - 2}
        }
    };
    Player mWhite {mElements.pieceGroup(white()), mElements.fieldGroup(whitedrawer()), mElements.fieldGroup(whiteprison())};
    Player mBlack {mElements.pieceGroup(black()), mElements.fieldGroup(blackdrawer()), mElements.fieldGroup(blackprison())};
    Player* mCurrent {&mWhite};
    Player* mOpponent {&mBlack};
    MuehlePiece* mSelectedPiece {nullptr};
};

#endif // SRC_MUEHLE_MUEHLE
