#include "muehlepiecemodel.h"
#include "muehlefieldmodel.h"
#include "muehlepieceview.h"

bool operator==(const MuehlePieceId& first, const MuehlePieceId& second)
{
    return (first.playerId == second.playerId && first.pieceNumber == second.pieceNumber);
}

MuehlePieceModel::MuehlePieceModel(size_t size)
    : mPieces(size, {"", -1})
{
}

void MuehlePieceModel::setField(MuehleFieldId f, size_t piecepos)
{
    if (piecepos < mPieces.size()) {
        mPieces[piecepos] = f;
        if (mView != nullptr) {
            mView->setField(f, piecepos);
        }
    }
}

MuehleFieldId MuehlePieceModel::field(size_t pos)
{
    if (pos < mPieces.size()) {
        return mPieces[pos];
    }
    return {"", -1};
}

MuehleFieldId MuehlePieceModel::back()
{
    return mPieces.back();
}

size_t MuehlePieceModel::size()
{
    return mPieces.size();
}

void MuehlePieceModel::setView(MuehlePieceView* p)
{
    if (mView == nullptr) {
        mView = p;
    }
}

void MuehlePieceModel::normalState(size_t pos)
{
    if (pos < mPieces.size() && mView != nullptr) {
        mView->normalState(pos);
    }
}

void MuehlePieceModel::canMoveState(size_t pos)
{
    if (pos < mPieces.size() && mView != nullptr) {
        mView->canMoveState(pos);
    }
}

void MuehlePieceModel::movingState(size_t pos)
{
    if (pos < mPieces.size() && mView != nullptr) {
        mView->movingState(pos);
    }
}

std::string MuehlePieceModel::color()
{
    return mView->color();
}
