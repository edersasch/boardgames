#include "muehlepiece.h"
#include "muehlepiecegroup.h"
#include "muehlepieceui.h"
#include "muehlefield.h"
#include "muehlefieldui.h"

MuehlePiece::MuehlePiece(MuehlePieceGroup* pGroup)
    : mPieceGroup(pGroup)
{
}

MuehleField* MuehlePiece::field() const
{
    return mField;
}

void MuehlePiece::setField(MuehleField* field)
{
    if (mField) {
        mField->setPiece(nullptr);
    }
    if (mField != field) {
        MuehleFieldUi* fUi = nullptr;
        mField = field;
        if (mField) {
            mField->setPiece(this);
            fUi = mField->fieldUi();
        }
        if (mPieceUi) {
            mPieceUi->fieldChanged(fUi);
        }
    }
}

MuehlePieceGroup* MuehlePiece::pieceGroup() const
{
    return mPieceGroup;
}

MuehlePieceUi* MuehlePiece::pieceUi() const
{
    return mPieceUi;
}

void MuehlePiece::setPieceUi(MuehlePieceUi* pieceUi)
{
    mPieceUi = pieceUi;
}

void MuehlePiece::pin()
{
    mAllowedAction = AllowedPieceActions::NONE;
    if (mPieceUi) {
        mPieceUi->pin();
    }
}

void MuehlePiece::selectable()
{
    mAllowedAction = AllowedPieceActions::SELECT;
    if (mPieceUi) {
        mPieceUi->selectable();
    }
}

void MuehlePiece::selected()
{
    if (mAllowedAction == AllowedPieceActions::SELECT) {
        mPieceGroup->selected(this);
    }
}

void MuehlePiece::removable()
{
    mAllowedAction = AllowedPieceActions::REMOVE;
    if (mPieceUi) {
        mPieceUi->removable();
    }
}

void MuehlePiece::removed()
{
    if (mAllowedAction == AllowedPieceActions::REMOVE) {
        mPieceGroup->removed(this);
    }
}
