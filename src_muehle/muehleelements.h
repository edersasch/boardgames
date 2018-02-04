#ifndef SRC_MUEHLE_MUEHLEELEMENTS
#define SRC_MUEHLE_MUEHLEELEMENTS

#include "muehlepiecegroup.h"
#include "muehlefieldgroup.h"

#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>

class MuehlePiece;

class MuehleElements
{
public:
    MuehleElements(const std::vector<std::pair<std::string, std::size_t>>& pieceGroups, const std::vector<std::pair<std::string, std::size_t>>& fieldGroups);
    virtual ~MuehleElements() = default;
    MuehlePieceGroup* pieceGroup(const std::string& pieceGroupName);
    MuehleFieldGroup* fieldGroup(const std::string& fieldGroupName);
    std::vector<MuehlePiece*> piecesInFieldGroup(const std::string& fieldGroupName);
    std::vector<MuehlePiece*> piecesOfGroupInFieldGroup(const std::string& pieceGroupName, const std::string& fieldGroupName);
private:
    std::unordered_map<std::string, std::unique_ptr<MuehlePieceGroup>> mPieceGroups {};
    std::unordered_map<std::string, std::unique_ptr<MuehleFieldGroup>> mFieldGroups {};
};

#endif // SRC_MUEHLE_MUEHLEELEMENTS
