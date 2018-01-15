#include "muehlefieldview.h"
#include "muehlefieldmodel.h"

void MuehleFieldView::setModel(MuehleFieldModel* f)
{
    if (mModel == nullptr) {
        mModel = f;
        mModel->setView(this);
    }
}

void MuehleFieldView::destinationSelected(int fieldNumber)
{
    if (mModel != nullptr) {
        mModel->destinationSelected(fieldNumber);
    }
}
