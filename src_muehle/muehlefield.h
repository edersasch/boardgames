#ifndef SRC_MUEHLE_MUEHLEFIELD
#define SRC_MUEHLE_MUEHLEFIELD

#include <string>

class MuehleFieldGroup;
class MuehleFieldUi;
class MuehlePiece;

class MuehleField
{
public:
    MuehleField(MuehleFieldGroup* fGroup, std::size_t fNumber);
    ~MuehleField() = default;
    MuehlePiece* piece() const;
    void setPiece(MuehlePiece* piece);
    MuehleFieldGroup* fieldGroup() const;
    const std::size_t& fieldNumber() const;
    MuehleFieldUi* fieldUi() const;
    void setFieldUi(MuehleFieldUi* fUi);
    void lock();
    void occupiable(std::string color);
    void occupy();
private:
    enum class AllowedFieldActions
    {
        NONE,
        OCCUPY
    };
    AllowedFieldActions mAllowedAction {AllowedFieldActions::NONE};
    MuehleFieldGroup* mFieldGroup;
    const std::size_t mFieldNumber;
    MuehlePiece* mPiece {nullptr};
    MuehleFieldUi* mFieldUi {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEFIELD
