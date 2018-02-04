#include "muehlepieceui.h"
#include "muehlepiece.h"
#include "muehlefield.h"

MuehlePieceUi::MuehlePieceUi(MuehlePiece* piece)
{
    mPiece = piece;
    piece->setPieceUi(this);
}
