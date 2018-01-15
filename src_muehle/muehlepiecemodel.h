#ifndef SRC_MUEHLE_MUEHLEPIECEMODEL
#define SRC_MUEHLE_MUEHLEPIECEMODEL

#include <utility>
#include <vector>
#include <string>
#include <cstddef>

class MuehleFieldId;
class MuehlePieceView;

struct MuehlePieceId
{
    std::string playerId;
    int pieceNumber;
};

bool operator==(const MuehlePieceId& first, const MuehlePieceId& second);

class MuehlePieceModel
{
public:
    MuehlePieceModel(std::size_t size);
    ~MuehlePieceModel() = default;
    void setField(MuehleFieldId f, std::size_t piecepos);
    MuehleFieldId field(std::size_t pos);
    MuehleFieldId back();
    std::size_t size();
    void setView(MuehlePieceView* p);
    void normalState(std::size_t pos);
    void canMoveState(std::size_t pos);
    void movingState(std::size_t pos);
    std::string color();
private:
    std::vector<MuehleFieldId> mPieces;
    MuehlePieceView* mView {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEPIECEMODEL
