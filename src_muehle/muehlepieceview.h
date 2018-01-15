#ifndef SRC_MUEHLE_MUEHLEPIECEVIEW
#define SRC_MUEHLE_MUEHLEPIECEVIEW

#include "muehlefieldmodel.h"

#include <string>
#include <cstddef>

class MuehlePieceModel;

class MuehlePieceView
{
public:
    MuehlePieceView(std::string color);
    virtual ~MuehlePieceView() = default;
    void setModel(MuehlePieceModel* p);
    std::string color();
    virtual void setField(MuehleFieldId f, std::size_t pos) = 0;
    virtual void normalState(std::size_t pos) = 0;
    virtual void canMoveState(std::size_t pos) = 0;
    virtual void movingState(std::size_t pos) = 0;
protected:
    MuehlePieceModel* mModel {nullptr};
    std::string mColor;
};

#endif // SRC_MUEHLE_MUEHLEPIECEVIEW
