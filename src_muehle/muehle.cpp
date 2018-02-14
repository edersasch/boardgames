#include "muehle.h"
#include "muehleui.h"

Muehle::Muehle(MuehleUi* gameUi)
    : mUi(gameUi)
{
    mElements.pieceGroup(white())->registerRemoved([this](MuehlePiece* piece) {
        pieceRemoved(piece);
    });
    mElements.pieceGroup(white())->registerSelected([this](MuehlePiece* piece) {
        pieceSelected(piece);
    });
    mElements.pieceGroup(black())->registerRemoved([this](MuehlePiece* piece) {
        pieceRemoved(piece);
    });
    mElements.pieceGroup(black())->registerSelected([this](MuehlePiece* piece) {
        pieceSelected(piece);
    });
    mElements.fieldGroup(board())->registerOccupy([this](MuehleField* field) {
        occupy(field);
    });
    mElements.fieldGroup(whitedrawer())->registerOccupy([this](MuehleField* field) {
        occupy(field);
    });
    mElements.fieldGroup(blackdrawer())->registerOccupy([this](MuehleField* field) {
        occupy(field);
    });
    mElements.fieldGroup(whiteprison())->registerOccupy([this](MuehleField* field) {
        occupy(field);
    });
    mElements.fieldGroup(blackprison())->registerOccupy([this](MuehleField* field) {
        occupy(field);
    });
}

void Muehle::newGame()
{
    mCurrent = &mWhite;
    mOpponent = &mBlack;
    for (std::size_t i = 0; i < numberOfPieces(); i += 1) {
        mWhite.mPieces->at(i)->setField(mWhite.mDrawer->at(i));
        mBlack.mPieces->at(i)->setField(mBlack.mDrawer->at(i));
    }
    leaveSetupMode();
}

MuehleElements* Muehle::muehleElements()
{
    return &mElements;
}

void Muehle::enterSetupMode()
{
    mSetupMode = true;
    occupiableEmptyFields();
    for (std::size_t i = 0; i < numberOfPieces(); i += 1) {
        mCurrent->mPieces->at(i)->selectable();
        mOpponent->mPieces->at(i)->selectable();
    }
    mCurrent->mDrawer->canHide(false);
    mCurrent->mPrison->canHide(false);
    mOpponent->mDrawer->canHide(false);
    mOpponent->mPrison->canHide(false);
}

void Muehle::leaveSetupMode()
{
    mSetupMode = false;
    bool wdrawerCanHide = true;
    bool bdrawerCanHide = true;
    bool wprisonCanHide = true;
    bool bprisonCanHide = true;
    for (std::size_t i = 0; i < numberOfPieces(); i += 1) {
        mElements.pieceGroup(white())->at(i)->pin();
        mElements.pieceGroup(black())->at(i)->pin();
    }
    lockBoardFields();
    for (std::size_t i = 0; i < numberOfPieces(); i+= 1) {
        mElements.fieldGroup(whitedrawer())->at(i)->lock();
        wdrawerCanHide &= mElements.fieldGroup(whitedrawer())->at(i)->piece() == nullptr;
        mElements.fieldGroup(blackdrawer())->at(i)->lock();
        bdrawerCanHide &= mElements.fieldGroup(blackdrawer())->at(i)->piece() == nullptr;
    }
    mElements.fieldGroup(whitedrawer())->canHide(wdrawerCanHide);
    mElements.fieldGroup(blackdrawer())->canHide(bdrawerCanHide);
    for (std::size_t i = 0; i < numberOfPieces() - 2; i+= 1) {
        mElements.fieldGroup(whiteprison())->at(i)->lock();
        wprisonCanHide &= mElements.fieldGroup(whiteprison())->at(i)->piece() == nullptr;
        mElements.fieldGroup(blackprison())->at(i)->lock();
        bprisonCanHide &= mElements.fieldGroup(blackprison())->at(i)->piece() == nullptr;
    }
    mElements.fieldGroup(whiteprison())->canHide(wprisonCanHide);
    mElements.fieldGroup(blackprison())->canHide(bprisonCanHide);
    startMove();
}

// private

void Muehle::occupy(MuehleField* field)
{
    if (!mSelectedPiece) {
        return;
    }
    if (mSelectedPiece->field()) {
        mSelectedPiece->field()->lock(); // ui can stop to highlight start position
    }
    mSelectedPiece->setField(field);
    if (mSetupMode) {
        occupiableEmptyFields();
        return;
    }
    lockBoardFields();
    for (auto& p : mElements.piecesInFieldGroup(board())) {
        p->pin();
    }
    mCurrent->mDrawer->canHide(mElements.piecesInFieldGroup(mCurrent == &mWhite ? whitedrawer() : blackdrawer()).size() == 0);
    if (closedMuehle(field->fieldNumber())) {
        bool removablePiecesAvailable = false;
        for (auto& p : mElements.piecesOfGroupInFieldGroup(mOpponent == &mWhite ? white() : black(), board())) {
            if (!closedMuehle(p->field()->fieldNumber())) {
                p->removable();
                removablePiecesAvailable = true;
            }
        }
        if (!removablePiecesAvailable) {
            swapPlayers();
        }
    } else {
        swapPlayers();
    }
}

void Muehle::pieceRemoved(MuehlePiece* removedPiece)
{
    for (auto& p : mElements.piecesOfGroupInFieldGroup(mOpponent == &mWhite ? white() : black(), board())) {
        p->pin();
    }
    for (std::size_t i = 0; i < mOpponent->mPrison->size(); i += 1) {
        if (!mOpponent->mPrison->at(i)->piece()) {
            mOpponent->mPrison->canHide(false);
            removedPiece->setField(mOpponent->mPrison->at(i));
            break;
        }
    }
    if (mElements.piecesInFieldGroup(mOpponent == &mWhite ? whiteprison() : blackprison()).size() == 7) {
        mUi->win(mCurrent == &mWhite ? white() : black());
    } else {
        swapPlayers();
    }
}

void Muehle::pieceSelected(MuehlePiece* selectedPiece)
{
    if (selectedPiece != mSelectedPiece) { // always false in phase 1
        if (mSelectedPiece && mSelectedPiece->field()) {
            mSelectedPiece->field()->lock(); // ui can stop to highlight start position
        }
        mSelectedPiece = selectedPiece;
        if (mSelectedPiece) {
            if (mSetupMode == true) {
                if (mSelectedPiece->pieceGroup() != mCurrent->mPieces) {
                    Player* tmp = mCurrent;
                    mCurrent = mOpponent;
                    mOpponent = tmp;
                    occupiableEmptyFields();
                }
            } else {
                if (mElements.piecesInFieldGroup(mCurrent == &mWhite ? whiteprison() : blackprison()).size() < 6) { // phase 2
                    lockBoardFields();
                    for (auto f : freeAdjacentFields(mSelectedPiece)) {
                        f->occupiable(mCurrent->mPieces->color());
                    }
                }
            }
            if (mSelectedPiece->field()) {
                mSelectedPiece->field()->highlight();
            }
        } // nothing to do for phase 3
    }
}

void Muehle::startMove()
{
    mSelectedPiece = nullptr;
    for (std::size_t i = 0; i < mCurrent->mDrawer->size(); i += 1) {
        if (mCurrent->mDrawer->at(i)->piece()) {
            mSelectedPiece = mCurrent->mDrawer->at(i)->piece();
            mSelectedPiece->selectable(); // phase 1: placement of new pieces
            if (mSelectedPiece->field()) {
                mSelectedPiece->field()->highlight(); // only one piece selectable, ui can highlight start position
            }
            occupiableEmptyBoardFields();
            return;
        }
    }
    if (mElements.piecesInFieldGroup(mCurrent == &mWhite ? whiteprison() : blackprison()).size() < 6) { // phase 2: more than three pieces available
        std::vector<MuehlePiece*> selectables;
        for (auto& p : mElements.piecesOfGroupInFieldGroup(mCurrent == &mWhite ? white() : black(), board())) {
            if (freeAdjacentFields(p).size() > 0) {
                p->selectable();
                selectables.push_back(p);
            }
        }
        if (selectables.size() <= 1) {
            if (selectables.size() == 0) {
                swapPlayers();
            } else {
                mSelectedPiece = selectables.back();
                if (mSelectedPiece->field()) {
                    mSelectedPiece->field()->highlight(); // only one piece selectable, ui can highlight start position
                }
            }
        }
    } else { // phase 3: last three pieces may jump
        for (auto& p : mElements.piecesOfGroupInFieldGroup(mCurrent == &mWhite ? white() : black(), board())) {
            p->selectable();
        }
        occupiableEmptyBoardFields();
    }
}

void Muehle::swapPlayers()
{
    Player* tmp = mCurrent;
    mCurrent = mOpponent;
    mOpponent = tmp;
    startMove();
}

bool Muehle::closedMuehle(int fieldNumber)
{
    MuehlePiece* one;
    MuehlePiece* two;
    MuehlePiece* three;
    auto muehleOnFields = [this, fieldNumber, &one, &two, &three] (int fieldOne, int fieldTwo, int fieldThree) {
        if (fieldNumber == fieldOne || fieldNumber == fieldTwo || fieldNumber == fieldThree) {
            one = mElements.fieldGroup(board())->at(fieldOne)->piece();
            two = mElements.fieldGroup(board())->at(fieldTwo)->piece();
            three = mElements.fieldGroup(board())->at(fieldThree)->piece();
            if (one && two && three &&
                    one->pieceGroup() == two->pieceGroup() &&
                    one->pieceGroup() == three->pieceGroup()) {
                return true;
            }
        }
        return false;
    };

    // horizontal
    if (fieldNumber % 3 == 0 &&
            muehleOnFields(fieldNumber, fieldNumber + 1, fieldNumber + 2)) {
        return true;
    } else if (fieldNumber % 3 == 1 &&
               muehleOnFields(fieldNumber, fieldNumber - 1, fieldNumber + 1)) {
        return true;
    } else if (fieldNumber % 3 == 2 &&
               muehleOnFields(fieldNumber, fieldNumber - 1, fieldNumber - 2)) {
        return true;
    }

    // vertical
    if (muehleOnFields(0, 9, 21)) {
        return true;
    } else if (muehleOnFields(3, 10, 18)) {
        return true;
    } else if (muehleOnFields(6, 11, 15)) {
        return true;
    } else if (muehleOnFields(1, 4, 7)) {
        return true;
    } else if (muehleOnFields(16, 19, 22)) {
        return true;
    } else if (muehleOnFields(8, 12, 17)) {
        return true;
    } else if (muehleOnFields(5, 13, 20)) {
        return true;
    } else {
        return muehleOnFields(2, 14, 23);
    }
}

std::vector<MuehleField*> Muehle::freeAdjacentFields(MuehlePiece* p)
{
    std::vector<MuehleField*> freeFields;
    auto checkFree = [this, &freeFields] (std::vector<std::size_t> numbers) {
        for (auto num : numbers) {
            if (!mElements.fieldGroup(board())->at(num)->piece()) {
                freeFields.push_back(mElements.fieldGroup(board())->at(num));
            }
        }
    };
    switch (p->field()->fieldNumber()) {
    case 0:
        checkFree({1, 9});
        break;
    case 1:
        checkFree({0, 2, 4});
        break;
    case 2:
        checkFree({1, 14});
        break;
    case 3:
        checkFree({4, 10});
        break;
    case 4:
        checkFree({1, 3, 5, 7});
        break;
    case 5:
        checkFree({4, 13});
        break;
    case 6:
        checkFree({7, 11});
        break;
    case 7:
        checkFree({4, 6, 8});
        break;
    case 8:
        checkFree({7, 12});
        break;
    case 9:
        checkFree({0, 10, 21});
        break;
    case 10:
        checkFree({3, 9, 11, 18});
        break;
    case 11:
        checkFree({6, 10, 15});
        break;
    case 12:
        checkFree({8, 13, 17});
        break;
    case 13:
        checkFree({5, 12, 14, 20});
        break;
    case 14:
        checkFree({2, 13, 23});
        break;
    case 15:
        checkFree({11, 16});
        break;
    case 16:
        checkFree({15, 17, 19});
        break;
    case 17:
        checkFree({12, 16});
        break;
    case 18:
        checkFree({10, 19});
        break;
    case 19:
        checkFree({16, 18, 20, 22});
        break;
    case 20:
        checkFree({13, 19});
        break;
    case 21:
        checkFree({9, 22});
        break;
    case 22:
        checkFree({19, 21, 23});
        break;
    case 23:
        checkFree({14, 22});
        break;
    default:
        break;
    }
    return freeFields;
}

void Muehle::lockBoardFields()
{
    for (std::size_t i = 0; i < numberOfFields(); i+= 1) {
        mElements.fieldGroup(board())->at(i)->lock();
    }
}

void Muehle::occupiableEmptyBoardFields()
{
    for (std::size_t i = 0; i < numberOfFields(); i+= 1) {
        if (!mElements.fieldGroup(board())->at(i)->piece()) {
            mElements.fieldGroup(board())->at(i)->occupiable(mCurrent->mPieces->color());
        }
    }
}

void Muehle::occupiableEmptyFields()
{
    occupiableEmptyBoardFields();
    for (std::size_t i = 0; i < numberOfPieces(); i+= 1) {
        if (!mCurrent->mDrawer->at(i)->piece()) {
            mCurrent->mDrawer->at(i)->occupiable(mCurrent->mPieces->color());
        }
        mOpponent->mDrawer->at(i)->lock();
    }
    for (std::size_t i = 0; i < numberOfPieces() - 2; i+= 1) {
        if (!mCurrent->mPrison->at(i)->piece()) {
            mCurrent->mPrison->at(i)->occupiable(mCurrent->mPieces->color());
        }
        mOpponent->mPrison->at(i)->lock();
    }
}
