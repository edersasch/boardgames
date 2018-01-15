#ifndef SRC_MUEHLE_MUEHLECONTROL
#define SRC_MUEHLE_MUEHLECONTROL

#include <string>

class MuehleControl
{
public:
    MuehleControl() = default;
    virtual ~MuehleControl() = default;
    virtual void draw() = 0;
    virtual void win(std::string playerId) = 0;
};

#endif // SRC_MUEHLE_MUEHLECONTROL
