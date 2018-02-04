#include "muehlefieldui.h"
#include "muehlefield.h"

MuehleFieldUi::MuehleFieldUi(MuehleField *field)
    : mField(field)
{
    field->setFieldUi(this);
}
