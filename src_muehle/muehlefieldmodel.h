#ifndef SRC_MUEHLE_MUEHLEFIELDMODEL
#define SRC_MUEHLE_MUEHLEFIELDMODEL

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <cstddef>

class MuehlePieceId;
class MuehleFieldView;

struct MuehleFieldId
{
    std::string fieldGroup;
    int fieldNumber;
};

bool operator==(const MuehleFieldId& first, const MuehleFieldId& second);

class MuehleFieldModel
{
public:
    MuehleFieldModel(std::size_t size);
    ~MuehleFieldModel() = default;
    void setPiece(MuehlePieceId p, std::size_t fieldpos);
    MuehlePieceId piece(std::size_t pos);
    MuehlePieceId back();
    std::size_t size();
    void setView(MuehleFieldView* f);
    void normalState(std::size_t pos);
    void destinationState(std::size_t pos, std::string color);
    void registerDestinationSelected(std::function<void(int)> callback);
    void destinationSelected(int fieldNumber);
private:
    std::vector<MuehlePieceId> mFields;
    MuehleFieldView* mView {nullptr};
    std::function<void(std::size_t)> mDestinationSelected;
};

#endif // SRC_MUEHLE_MUEHLEFIELDMODEL
