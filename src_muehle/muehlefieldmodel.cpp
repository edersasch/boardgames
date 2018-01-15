#include "muehlefieldmodel.h"
#include "muehlepiecemodel.h"
#include "muehlefieldview.h"

bool operator==(const MuehleFieldId& first, const MuehleFieldId& second)
{
    return (first.fieldGroup == second.fieldGroup && first.fieldNumber == second.fieldNumber);
}

MuehleFieldModel::MuehleFieldModel(size_t size)
    : mFields(size, {"", -1})
{
}

void MuehleFieldModel::setPiece(MuehlePieceId p, size_t fieldpos)
{
    if (fieldpos < mFields.size()) {
        mFields[fieldpos] = p;
        if (mView != nullptr) {
            mView->setPiece(p, fieldpos);
        }
    }
}

MuehlePieceId MuehleFieldModel::piece(size_t pos)
{
    if (pos < mFields.size()) {
        return mFields[pos];
    }
    return {"", -1};
}

MuehlePieceId MuehleFieldModel::back()
{
    return mFields.back();
}

size_t MuehleFieldModel::size()
{
    return mFields.size();
}

void MuehleFieldModel::setView(MuehleFieldView* f)
{
    if (mView == nullptr) {
        mView = f;
    }
}

void MuehleFieldModel::normalState(size_t pos)
{
    if (pos < mFields.size() && mView != nullptr) {
        mView->normalState(pos);
    }
}

void MuehleFieldModel::destinationState(size_t pos, std::string color)
{
    if (pos < mFields.size() && mView != nullptr) {
        mView->destinationState(pos, color);
    }
}

void MuehleFieldModel::registerDestinationSelected(std::function<void(int)> callback)
{
    mDestinationSelected = callback;
}

void MuehleFieldModel::destinationSelected(int fieldNumber)
{
    mDestinationSelected(fieldNumber);
}
