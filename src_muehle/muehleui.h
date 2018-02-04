#ifndef SRC_MUEHLE_MUEHLEUI
#define SRC_MUEHLE_MUEHLEUI

#include <string>

class MuehleUi
{
public:
    MuehleUi() = default;
    virtual ~MuehleUi() = default;
    virtual void draw() = 0;
    virtual void win(const std::string& playerId) = 0;
};

#endif // SRC_MUEHLE_MUEHLEUI
