#include "muehlepieceview.h"
#include "muehlepiecemodel.h"

MuehlePieceView::MuehlePieceView(std::string color)
    : mColor(color)
{
}

void MuehlePieceView::setModel(MuehlePieceModel* p)
{
    if (mModel == nullptr) {
        mModel = p;
        mModel->setView(this);
    }
}

std::string MuehlePieceView::color()
{
    return mColor;
}
