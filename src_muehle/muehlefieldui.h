#ifndef SRC_MUEHLE_MUEHLEFIELDUI
#define SRC_MUEHLE_MUEHLEFIELDUI

#include <string>

class MuehleField;
class MuehlePieceUi;

class MuehleFieldUi
{
public:
    MuehleFieldUi(MuehleField* field);
    virtual ~MuehleFieldUi() = default;
    virtual void lock() = 0;
    virtual void occupiable(const std::string& color) = 0;
protected:
    MuehleField* mField {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEFIELDUI
