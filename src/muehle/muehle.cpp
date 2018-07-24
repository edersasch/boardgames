#include "muehle.h"

#include <iostream>

void Muehle::new_game()
{
    mCurrent = &mWhite;
    mOpponent = &mBlack;
    std::fill(mFields.begin(), mFields.end(), boardgame::noPiece);
    std::fill(mConstellation.begin(), mConstellation.end(), boardgame::noField);
    for (int i = 0; i < numberOfPiecesPerPlayer.v; i += 1) {
        set_field_helper(boardgame::PieceNumber{firstWhitePiece.v + i}, boardgame::FieldNumber{firstWhiteDrawerField.v + i});
        set_field_helper(boardgame::PieceNumber{firstBlackPiece.v + i}, boardgame::FieldNumber{firstBlackDrawerField.v + i});
    }
    mSetupMode = true;
    leave_setup_mode();
}

void Muehle::enter_setup_mode()
{
    mSetupMode = true;
    mSelectablePieces.clear();
    mOccupiableFields.clear();
    occupiable_empty_fields();
    for (int i = 0; i < numberOfPieces.v; i += 1) {
        mSelectablePieces.push_back(boardgame::PieceNumber{i});
        selectable(mUi, boardgame::PieceNumber{i});
    }
    white_drawer_can_hide(mUi, false);
    white_prison_can_hide(mUi, false);
    black_drawer_can_hide(mUi, false);
    black_prison_can_hide(mUi, false);
}

void Muehle::leave_setup_mode()
{
    if (mSetupMode) {
        move_list = std::make_unique<boardgame::Move_List<MuehleConstellation, Move_List_Ui>>(mlUi);
        move_list->commit(mConstellation, std::string(), {mCurrent == &mWhite ? firstBlackPiece.v : firstWhitePiece.v}); // initial position, hint is the potential piece that "moved"
    }
    mSelectablePieces.clear();
    mOccupiableFields.clear();
    mSetupMode = false;
    lock_pieces(mUi);
    lock_fields(make_fieldgroup(mFields, firstBoardField, numberOfFields));
    white_drawer_can_hide(mUi, is_fieldgroup_empty(mWhiteDrawer));
    black_drawer_can_hide(mUi, is_fieldgroup_empty(mBlackDrawer));
    white_prison_can_hide(mUi, is_fieldgroup_empty(mWhitePrison));
    black_prison_can_hide(mUi, is_fieldgroup_empty(mBlackPrison));
    start_move();
}

void Muehle::piece_removed(const boardgame::PieceNumber id)
{
    if (std::find(mSelectablePieces.begin(), mSelectablePieces.end(), id) != mSelectablePieces.end()) {
        mSelectablePieces.clear();
        lock_pieces(mUi);
        if (mOpponent == &mWhite) {
            white_prison_can_hide(mUi, false);
        } else {
            black_prison_can_hide(mUi, false);
        }
        set_field_helper(id, first_empty_field(mOpponent->mPrisonGroup));
        if (first_empty_field(mOpponent->mPrisonGroup) == boardgame::noField) {
            win(mUi, mCurrent == &mWhite ? std::string("white") : std::string("black"));
        } else {
            swap_players();
        }
    }
}

void Muehle::piece_selected(const boardgame::PieceNumber id)
{
    if (std::find(mSelectablePieces.begin(), mSelectablePieces.end(), id) != mSelectablePieces.end() && id != mSelectedPiece) { // always false in phase 1
        if (mSelectedPiece != boardgame::noPiece) {
            lock_field(mUi, mConstellation.at(static_cast<std::size_t>(mSelectedPiece.v))); // ui can stop to highlight start position
        }
        mSelectedPiece = id;
        if (mSetupMode) {
            if (!is_in_group(mSelectedPiece, mCurrent->mPieceGroup)) {
                const Player* tmp = mCurrent;
                mCurrent = mOpponent;
                mOpponent = tmp;
                occupiable_empty_fields();
            }
        } else {
            if (number_of_pieces_in_fieldgroup(mCurrent->mPrisonGroup) < 6) { // phase 2
                lock_fields(mBoard);
                for (auto f : free_adjacent_fields(mConstellation.at(static_cast<std::size_t>(mSelectedPiece.v)))) {
                    mOccupiableFields.push_back(f);
                    occupiable(mUi, f, mSelectedPiece);
                }
            }
        }
        highlight(mUi, mConstellation.at(static_cast<std::size_t>(mSelectedPiece.v)));
    } // nothing to do for phase 3
}

void Muehle::occupy(const boardgame::FieldNumber id)
{
    if (mSelectedPiece == boardgame::noPiece || std::find(mOccupiableFields.begin(), mOccupiableFields.end(), id) == mOccupiableFields.end()) {
        return;
    }
    mOccupiableFields.clear();
    set_field_helper(mSelectedPiece, id);
    lock_fields(make_fieldgroup(mFields, firstBoardField, numberOfFields));
    if (mSetupMode) {
        occupiable_empty_fields();
        highlight(mUi, id);
        return;
    }
    mSelectablePieces.clear();
    lock_pieces(mUi);
    if (mCurrent == &mWhite) {
        white_drawer_can_hide(mUi, is_fieldgroup_empty(mWhiteDrawer));
    } else {
        black_drawer_can_hide(mUi, is_fieldgroup_empty(mBlackDrawer));
    }
    if (closed_muehle(mConstellation.at(static_cast<std::size_t>(mSelectedPiece.v)))) {
        for (const auto& f : positions_of_pieces_in_fieldgroup(mOpponent->mPieceGroup, mBoard)) {
            if (!closed_muehle(f)) {
                mSelectablePieces.push_back(mFields.at(static_cast<std::size_t>(f.v)));
                removable(mUi, mFields.at(static_cast<std::size_t>(f.v)));
            }
        }
        if (mSelectablePieces.empty()) {
            swap_players();
        }
    } else {
        swap_players();
    }
}

void Muehle::request_set_current_move_and_branch_start_id(const int move_id)
{
    if (move_list->set_current_move_and_branch_start_id(move_id)) {
        update_game();
    }
}

void Muehle::request_cut_off(const int move_id)
{
    if (move_list->cut_off(move_id)) {
        update_game();
    }
}

void Muehle::request_move_list_forward()
{
    if (move_list->move_list_forward()) {
        update_game();
    }
}

void Muehle::request_move_list_back()
{
    if (move_list->move_list_back()) {
        update_game();
    }
}

// private

void Muehle::start_move()
{
    mSelectedPiece = first_piece(mCurrent->mDrawerGroup);
    if (mSelectedPiece != boardgame::noPiece) { // phase 1: placement of new pieces;
        selectable(mUi, mSelectedPiece);
        highlight(mUi, mConstellation.at(static_cast<std::size_t>(mSelectedPiece.v))); // only one piece selectable, ui can highlight start position
        occupiable_empty_board_fields();
        return;
    }
    if (number_of_pieces_in_fieldgroup(mCurrent->mPrisonGroup) < 6) { // phase 2: more than three pieces available
        for (const auto& f : positions_of_pieces_in_fieldgroup(mCurrent->mPieceGroup, mBoard)) {
            if (!free_adjacent_fields(f).empty()) {
                mSelectablePieces.push_back(mFields.at(static_cast<std::size_t>(f.v)));
                selectable(mUi, mSelectablePieces.back());
            }
        }
        if (mSelectablePieces.size() <= 1) {
            if (mSelectablePieces.empty()) {
                swap_players();
            } else {
                mSelectedPiece = mSelectablePieces.back();
                mSelectablePieces.pop_back();
                occupiable_empty_board_fields();
            }
        }
    } else { // phase 3: last three pieces may jump
        for (const auto& f : positions_of_pieces_in_fieldgroup(mCurrent->mPieceGroup, mBoard)) {
            mSelectablePieces.push_back(mFields.at(static_cast<std::size_t>(f.v)));
            selectable(mUi, mSelectablePieces.back());
        }
        occupiable_empty_board_fields();
    }
}

void Muehle::swap_players()
{
    std::string commitmsg = "--"; // used in case the player was not able to move
    boardgame::PieceNumber movedPiece = boardgame::noPiece;
    if (auto diffed = diff(move_list->constellation(), mConstellation); !diffed.empty()) { // example "21-22 x18" means: moved from field 21 to field 22, removed piece from field 18
        std::string from;
        std::string to;
        std::string removed;
        for (const auto& d : diffed) {
            if (is_in_group(d.second.at(1), mBoard)) {
                movedPiece = d.first;
                to = std::to_string(d.second.at(1).v);
                if (is_in_group(d.second.at(0), mBoard)) {
                    from = std::to_string(d.second.at(0).v) + "-";
                }
            } else {
                if (is_in_group(d.second.at(1), mOpponent->mPrisonGroup)) {
                    removed = " x" + std::to_string(d.second.at(0).v);
                }
            }
        }
        commitmsg = from + to + removed;
    }
    move_list->commit(mConstellation, commitmsg, {movedPiece.v});
    const Player* tmp = mCurrent;
    mCurrent = mOpponent;
    mOpponent = tmp;
    start_move();
}

bool Muehle::closed_muehle(const boardgame::FieldNumber fieldNumber) const
{
    auto muehleOnFields = [this] (int fieldOne, int fieldTwo, int fieldThree) {
        boardgame::PieceNumber one = mFields.at(static_cast<std::size_t>(fieldOne));
        boardgame::PieceNumber two = mFields.at(static_cast<std::size_t>(fieldTwo));
        boardgame::PieceNumber three = mFields.at(static_cast<std::size_t>(fieldThree));
        return (one != boardgame::noPiece && two != boardgame::noPiece && three != boardgame::noPiece &&
                is_in_group(one, mWhitePieces) == is_in_group(two, mWhitePieces) &&
                is_in_group(one, mWhitePieces) == is_in_group(three, mWhitePieces));
    };

    if (muehleOnFields(fieldNumber.v, mVerticalMuehle.at(static_cast<std::size_t>(fieldNumber.v)).at(0), mVerticalMuehle.at(static_cast<std::size_t>(fieldNumber.v)).at(1))) { // vertical
        return true;
    }

    if (fieldNumber.v % 3 == 0) { // horizontal
        return muehleOnFields(fieldNumber.v, fieldNumber.v + 1, fieldNumber.v + 2);
    }
    if (fieldNumber.v % 3 == 1) {
        return muehleOnFields(fieldNumber.v, fieldNumber.v - 1, fieldNumber.v + 1);
    }
    return muehleOnFields(fieldNumber.v, fieldNumber.v - 1, fieldNumber.v - 2);
}

const std::vector<boardgame::FieldNumber> Muehle::free_adjacent_fields(const boardgame::FieldNumber fn) const
{
    std::vector<boardgame::FieldNumber> freeFields;
    for (const auto& num : mAdjacentFields.at(static_cast<std::size_t>(fn.v))) {
        if (mFields.at(static_cast<std::size_t>(firstBoardField.v + num)) == boardgame::noPiece) {
            freeFields.push_back(boardgame::FieldNumber{num});
        }
    }
    return freeFields;
}

void Muehle::lock_fields(const boardgame::Fieldgroup<decltype(mFields.cbegin())>& fg) const
{
    for (auto b = fg.b; b != fg.e; b += 1) {
        lock_field(mUi, boardgame::FieldNumber{static_cast<int>(b - fg.o)}); // narrowing is ok as long as you don't have more than 2 million elements
    }
}

void Muehle::occupiable_empty_board_fields()
{
    for (const auto& f : all_free_fields(mBoard)) {
        mOccupiableFields.push_back(f);
        occupiable(mUi, f, mSelectedPiece);
    }
}

void Muehle::occupiable_empty_fields()
{
    occupiable_empty_board_fields();
    for (const auto& f : all_free_fields(mCurrent->mDrawerGroup)) {
        mOccupiableFields.push_back(f);
        occupiable(mUi, f, mSelectedPiece);
    }
    for (const auto& f : all_free_fields(mCurrent->mPrisonGroup)) {
        mOccupiableFields.push_back(f);
        occupiable(mUi, f, mSelectedPiece);
    }
    lock_fields(mOpponent->mDrawerGroup);
    lock_fields(mOpponent->mPrisonGroup);
}

void Muehle::set_field_helper(const boardgame::PieceNumber pn, const boardgame::FieldNumber fn)
{
    if (const boardgame::FieldNumber oldfield = mConstellation.at(static_cast<std::size_t>(pn.v)); oldfield != boardgame::noField) {
        mFields.at(static_cast<std::size_t>(oldfield.v)) = boardgame::noPiece;
    }
    mConstellation.at(static_cast<std::size_t>(pn.v)) = fn;
    mFields.at(static_cast<std::size_t>(fn.v)) = pn;
    set_field(mUi, pn, fn);
}

void Muehle::update_game()
{
    auto& constellation = move_list->constellation();
    std::fill(mFields.begin(), mFields.end(), boardgame::noPiece);
    std::fill(mConstellation.begin(), mConstellation.end(), boardgame::noField);
    for (int i = 0; i < static_cast<int>(constellation.size()); i += 1) {
        set_field_helper(boardgame::PieceNumber{i}, constellation.at(static_cast<std::size_t>(i)));
    }
    mCurrent = &mWhite;
    mOpponent = &mBlack;
    auto& hint = move_list->hint();
    if (!hint.empty()) {
        if (hint.at(0) < firstBlackPiece.v) {
            mCurrent = &mBlack;
            mOpponent = &mWhite;
        }
    }
    leave_setup_mode();
}
