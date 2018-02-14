#include "muehlefieldgroup.h"

MuehleFieldGroup::MuehleFieldGroup(std::size_t size)
{
    for (std::size_t i = 0; i < size; i += 1) {
        mFields.emplace_back(MuehleField(this, i));
    }
}

MuehleField* MuehleFieldGroup::at(std::size_t pos)
{
    return &mFields.at(pos);
}

std::size_t MuehleFieldGroup::size() const
{
    return mFields.size();
}

void MuehleFieldGroup::occupy(MuehleField* field)
{
    if (mOccupyCallback) {
        mOccupyCallback(field);
    }
}

void MuehleFieldGroup::registerOccupy(std::function<void(MuehleField*)> occupyCallback)
{
    mOccupyCallback = occupyCallback;
}

void MuehleFieldGroup::canHide(bool hide)
{
    if (mCanHideCallback) {
        mCanHideCallback(hide);
    }
}

void MuehleFieldGroup::registerCanHide(std::function<void(bool)> canHideCallback)
{
    mCanHideCallback = canHideCallback;
}
