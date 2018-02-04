#include "muehlepiecegroup.h"

MuehlePieceGroup::MuehlePieceGroup(std::size_t size)
{
    for (std::size_t i = 0; i < size; i += 1) {
        mPieces.emplace_back(MuehlePiece(this));
    }
}

MuehlePiece* MuehlePieceGroup::at(std::size_t pos)
{
    return &mPieces.at(pos);
}

std::size_t MuehlePieceGroup::size() const
{
    return mPieces.size();
}

const std::string& MuehlePieceGroup::color() const
{
    return mColor;
}

void MuehlePieceGroup::setColor(const std::string& newColor)
{
    mColor = newColor;
}

void MuehlePieceGroup::removed(MuehlePiece* piece)
{
    if (mRemovedCallback) {
        mRemovedCallback(piece);
    }
}

void MuehlePieceGroup::registerRemoved(std::function<void(MuehlePiece*)> removedCallback)
{
    mRemovedCallback = removedCallback;
}

void MuehlePieceGroup::selected(MuehlePiece* piece)
{
    if (mSelectedCallback) {
        mSelectedCallback(piece);
    }
}

void MuehlePieceGroup::registerSelected(std::function<void(MuehlePiece*)> selectedCallback)
{
    mSelectedCallback = selectedCallback;
}
