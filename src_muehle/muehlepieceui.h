#ifndef SRC_MUEHLE_MUEHLEPIECEUI
#define SRC_MUEHLE_MUEHLEPIECEUI

class MuehlePiece;
class MuehleFieldUi;

class MuehlePieceUi
{
public:
    MuehlePieceUi(MuehlePiece* piece);
    virtual ~MuehlePieceUi() = default;
    virtual void fieldChanged(MuehleFieldUi* fUi) = 0;
    virtual void pin() = 0;
    virtual void selectable() = 0;
    virtual void removable() = 0;
protected:
    MuehlePiece* mPiece {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEPIECEUI
