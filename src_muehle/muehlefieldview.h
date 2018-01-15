#ifndef SRC_MUEHLE_MUEHLEFIELDVIEW
#define SRC_MUEHLE_MUEHLEFIELDVIEW

#include "muehlepiecemodel.h"

#include <string>
#include <cstddef>

class MuehleFieldModel;

class MuehleFieldView
{
public:
    MuehleFieldView() = default;
    virtual ~MuehleFieldView() = default;
    void setModel(MuehleFieldModel* f);
    virtual void destinationSelected(int fieldNumber);
    virtual void setPiece(MuehlePieceId p, std::size_t pos) = 0;
    virtual void normalState(std::size_t pos) = 0;
    virtual void destinationState(std::size_t pos, const std::string& color) = 0;
private:
    MuehleFieldModel* mModel {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEFIELDVIEW
