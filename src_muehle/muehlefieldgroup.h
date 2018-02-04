#ifndef SRC_MUEHLE_MUEHLEFIELDGROUP
#define SRC_MUEHLE_MUEHLEFIELDGROUP

#include "muehlefield.h"

#include <vector>
#include <functional>
#include <cstddef>

class MuehleFieldGroup
{
public:
    explicit MuehleFieldGroup(std::size_t size);
    ~MuehleFieldGroup() = default;
    MuehleField* at(std::size_t pos);
    std::size_t size() const;
    void occupy(MuehleField* field);
    void registerOccupy(std::function<void(MuehleField*)> occupyCallback);
private:
    std::vector<MuehleField> mFields {};
    std::function<void(MuehleField*)> mOccupyCallback {};
};

#endif // SRC_MUEHLE_MUEHLEFIELDGROUP
