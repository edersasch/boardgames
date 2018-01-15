#include "muehle.h"
#include "muehlepieceview.h"
#include "muehlefieldview.h"
#include "muehlecontrol.h"

Muehle::Muehle(MuehleControl* control,
               MuehlePieceView* whitePieceView, MuehlePieceView* blackPieceView,
               MuehleFieldView* whiteDrawerView, MuehleFieldView* blackDrawerView,
               MuehleFieldView* whitePrisonView, MuehleFieldView* blackPrisonView,
               MuehleFieldView* boardView)
    : mControl(control)
{
    mFields.at(board()).registerDestinationSelected([this](int fieldNumber) {
        destinationSelected(fieldNumber);
    });
    whitePieceView->setModel(&mPieces.at(white()));
    blackPieceView->setModel(&mPieces.at(black()));
    whiteDrawerView->setModel(&mFields.at(whitedrawer()));
    blackDrawerView->setModel(&mFields.at(blackdrawer()));
    whitePrisonView->setModel(&mFields.at(whiteprison()));
    blackPrisonView->setModel(&mFields.at(blackprison()));
    boardView->setModel(&mFields.at(board()));
}

void Muehle::newGame()
{
    mCurrent = &mWhite;
    mOpponent = &mBlack;
    for (int i = 0; i < NUMBER_OF_PIECES; i += 1) {
        movePiece({white(), i}, {whitedrawer(), i});
        mPieces.at(white()).normalState(i);
        movePiece({black(), i}, {blackdrawer(), i});
        mPieces.at(black()).normalState(i);
    }
    for (int i = 0; i < NUMBER_OF_FIELDS; i+= 1) {
        mFields.at(board()).normalState(i);
    }
    startMove();
}

void Muehle::destinationSelected(int fieldNumber)
{
    for (int i = 0; i < NUMBER_OF_FIELDS; i+= 1) {
        mFields.at(board()).normalState(i);
    }
    for (int i = 0; i < NUMBER_OF_PIECES; i += 1) {
        mCurrent->mPieces.normalState(i);
        mOpponent->mPieces.normalState(i);
    }
    movePiece(mSelectedPiece, {board(), fieldNumber});
    if(false) { // player closed muehle with this move
        ;
    } else {
        if (mCurrent == &mWhite) {
            mCurrent = &mBlack;
            mOpponent = &mWhite;
        } else {
            mCurrent = &mWhite;
            mOpponent = &mBlack;
        }
        startMove();
    }
}

// private

void Muehle::startMove()
{
    if (mCurrent->mDrawer.back().playerId != "" && mCurrent->mDrawer.back().pieceNumber != -1) { // phase 1: placement of new pieces
        for (size_t i = 0; i < mCurrent->mDrawer.size(); i += 1) {
            if (mCurrent->mDrawer.piece(i).playerId != "" && mCurrent->mDrawer.piece(i).pieceNumber != -1) {
                mSelectedPiece = mCurrent->mDrawer.piece(i);
                mCurrent->mPieces.movingState(mSelectedPiece.pieceNumber);
                mOnlyOnePieceSelectable = true;
                break;
            }
        }
        for (int i = 0; i < NUMBER_OF_FIELDS; i+= 1) {
            if (mFields.at(board()).piece(i).playerId == "" || mFields.at(board()).piece(i).pieceNumber == -1) {
                mFields.at(board()).destinationState(i, mCurrent->mPieces.color());
            }
        }
    } else if (mOpponent->mPrison.back().playerId == "" && mOpponent->mPrison.back().pieceNumber == -1) { // phase 2: more than three pieces available
        exit(0);
    } else { // phase 3: last three pieces may jump
        exit(0);
    }
}

void Muehle::movePiece(MuehlePieceId p, MuehleFieldId f)
{
    if (p.playerId == "" || p.pieceNumber == -1) {
        return;
    }
    MuehleFieldId oldf = mPieces.at(p.playerId).field(p.pieceNumber);
    if (oldf.fieldGroup != "" && oldf.fieldNumber != -1) {
        mFields.at(oldf.fieldGroup).setPiece({"", -1}, oldf.fieldNumber);
    }
    if (f.fieldGroup != "" && f.fieldNumber != -1) {
        mFields.at(f.fieldGroup).setPiece(p, f.fieldNumber);
        mPieces.at(p.playerId).setField(f, p.pieceNumber);
    }
}
