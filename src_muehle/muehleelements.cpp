#include "muehleelements.h"
#include "muehlepiece.h"

MuehleElements::MuehleElements(const std::vector<std::pair<std::string, std::size_t>>& pieceGroups, const std::vector<std::pair<std::string, std::size_t>>& fieldGroups)
{
    for (auto pg : pieceGroups) {
        mPieceGroups.emplace(pg.first, std::make_unique<MuehlePieceGroup>(pg.second));
    }
    for (auto fg : fieldGroups) {
        mFieldGroups.emplace(fg.first, std::make_unique<MuehleFieldGroup>(fg.second));
    }
}

MuehlePieceGroup* MuehleElements::pieceGroup(const std::string& pieceGroupName)
{
    return mPieceGroups.at(pieceGroupName).get();
}

MuehleFieldGroup* MuehleElements::fieldGroup(const std::string& fieldGroupName)
{
    return mFieldGroups.at(fieldGroupName).get();
}

std::vector<MuehlePiece*> MuehleElements::piecesInFieldGroup(const std::string& fieldGroupName)
{
    std::vector<MuehlePiece*> p;
    for (std::size_t i = 0; i < mFieldGroups.at(fieldGroupName)->size(); i += 1) {
        if (mFieldGroups.at(fieldGroupName)->at(i)->piece()) {
            p.push_back(mFieldGroups.at(fieldGroupName)->at(i)->piece());
        }
    }
    return p;
}

std::vector<MuehlePiece*> MuehleElements::piecesOfGroupInFieldGroup(const std::string& pieceGroupName, const std::string& fieldGroupName)
{
    std::vector<MuehlePiece*> p;
    MuehleFieldGroup* fg = mFieldGroups.at(fieldGroupName).get();
    MuehlePieceGroup* pg = mPieceGroups.at(pieceGroupName).get();
    for (std::size_t i = 0; i < pg->size(); i += 1) {
        if (pg->at(i)->field()->fieldGroup() == fg) {
            p.push_back(pg->at(i));
        }
    }
    return p;
}
