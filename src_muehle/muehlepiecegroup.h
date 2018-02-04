#ifndef SRC_MUEHLE_MUEHLEPIECEGROUP
#define SRC_MUEHLE_MUEHLEPIECEGROUP

#include "muehlepiece.h"

#include <vector>
#include <functional>
#include <cstddef>

class MuehlePieceGroup
{
public:
    explicit MuehlePieceGroup(std::size_t size);
    ~MuehlePieceGroup() = default;
    MuehlePiece* at(std::size_t pos);
    std::size_t size() const;
    const std::string& color() const;
    void setColor(const std::string& newColor);
    void removed(MuehlePiece* piece);
    void registerRemoved(std::function<void(MuehlePiece*)> removedCallback);
    void selected(MuehlePiece* piece);
    void registerSelected(std::function<void(MuehlePiece*)> selectedCallback);
private:
    std::vector<MuehlePiece> mPieces {};
    std::string mColor {};
    std::function<void(MuehlePiece*)> mRemovedCallback {nullptr};
    std::function<void(MuehlePiece*)> mSelectedCallback {nullptr};
};

#endif // SRC_MUEHLE_MUEHLEPIECEGROUP
