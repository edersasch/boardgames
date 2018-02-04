#ifndef SRC_MUEHLE_MUEHLEPIECE
#define SRC_MUEHLE_MUEHLEPIECE

#include <string>

class MuehlePieceGroup;
class MuehlePieceUi;
class MuehleField;

class MuehlePiece
{
public:
    MuehlePiece(MuehlePieceGroup* pGroup);
    ~MuehlePiece() = default;
    MuehleField* field() const;
    void setField(MuehleField* field);
    MuehlePieceGroup* pieceGroup() const;
    MuehlePieceUi* pieceUi() const;
    void setPieceUi(MuehlePieceUi* pUi);
    void pin();
    void selectable();
    void selected();
    void removable();
    void removed();
private:
    enum class AllowedPieceActions
    {
        NONE,
        SELECT,
        REMOVE
    };
    AllowedPieceActions mAllowedAction {AllowedPieceActions::NONE};
    MuehlePieceGroup* mPieceGroup;
    MuehleField* mField {nullptr};
    MuehlePieceUi* mPieceUi {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEPIECE
