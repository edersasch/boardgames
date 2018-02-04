#include "muehlefield.h"
#include "muehlefieldgroup.h"
#include "muehlefieldui.h"
#include "muehlepiece.h"

MuehleField::MuehleField(MuehleFieldGroup* fGroup, std::size_t fNumber)
    : mFieldGroup(fGroup)
    , mFieldNumber(fNumber)
{
}

MuehlePiece* MuehleField::piece() const
{
    return mPiece;
}

void MuehleField::setPiece(MuehlePiece* piece)
{
    mPiece = piece;
}

MuehleFieldGroup* MuehleField::fieldGroup() const
{
    return mFieldGroup;
}

const std::size_t& MuehleField::fieldNumber() const
{
    return mFieldNumber;
}

MuehleFieldUi* MuehleField::fieldUi() const
{
    return mFieldUi;
}

void MuehleField::setFieldUi(MuehleFieldUi* fUi)
{
    mFieldUi = fUi;
}

void MuehleField::lock()
{
    mAllowedAction = AllowedFieldActions::NONE;
    if (mFieldUi) {
        mFieldUi->lock();
    }
}

void MuehleField::occupiable(std::string color)
{
    mAllowedAction = AllowedFieldActions::OCCUPY;
    if (mFieldUi) {
        mFieldUi->occupiable(color);
    }
}

void MuehleField::occupy()
{
    if (mAllowedAction == AllowedFieldActions::OCCUPY) {
        mFieldGroup->occupy(this);
    }
}
