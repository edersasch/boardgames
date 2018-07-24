#ifndef SRC_MUEHLE_MUEHLE
#define SRC_MUEHLE_MUEHLE

#include "muehleui.h"
#include "boardgame/move_list_ui.h"
#include "boardgame/pieces_n_fields.h"
#include "boardgame/move_list.h"

#include <array>
#include <vector>
#include <memory>

/** Model setup and rule checking
 *
 * Field numbers:
 *
 *     0--------1--------2
 *     |        |        |
 *     |  3-----4-----5  |
 *     |  |     |     |  |
 *     |  |  6--7--8  |  |
 *     9-10-11    12-13-14
 *     |  | 15-16-17  |  |
 *     |  |     |     |  |
 *     |  18---19----20  |
 *     |        |        |
 *     21------22-------23
 *
 *     24 - 32: white drawer
 *     33 - 41: black drawer
 *     42 - 48: white prison
 *     49 - 55: black prison
 *
 * Piece numbers:
 *
 *     0 -  8: white pieces
 *     9 - 17: black pieces
*/

class Muehle
{
public:
    Muehle(Muehle_Ui_Type gameUi, Move_List_Ui movelistUi) : mUi(std::move(gameUi)), mlUi(std::move(movelistUi)) {}
    using MuehleConstellation = std::array<boardgame::FieldNumber, 18>; // number of pieces
    static constexpr boardgame::PieceRange numberOfPiecesPerPlayer   {9};
    static constexpr boardgame::PieceRange numberOfPieces          = numberOfPiecesPerPlayer + numberOfPiecesPerPlayer;
    static constexpr boardgame::PieceNumber firstWhitePiece          {0};
    static constexpr boardgame::PieceNumber firstBlackPiece        = firstWhitePiece + numberOfPiecesPerPlayer;
    static constexpr boardgame::FieldRange numberOfBoardFields       {24};
    static constexpr boardgame::FieldRange numberOfDrawerFields      {9}; // as much fields as numberOfPiecesPerPlayer
    static constexpr boardgame::FieldRange numberOfPrisonFields      {7}; // game ends if only two pieces left, so two less than numberOfDrawerFields
    static constexpr boardgame::FieldRange numberOfFields          = numberOfBoardFields + numberOfDrawerFields + numberOfDrawerFields + numberOfPrisonFields + numberOfPrisonFields;
    static constexpr boardgame::FieldNumber firstBoardField          {0};
    static constexpr boardgame::FieldNumber firstWhiteDrawerField  = firstBoardField + numberOfBoardFields;
    static constexpr boardgame::FieldNumber firstBlackDrawerField  = firstWhiteDrawerField + numberOfDrawerFields;
    static constexpr boardgame::FieldNumber firstWhitePrisonField  = firstBlackDrawerField + numberOfDrawerFields;
    static constexpr boardgame::FieldNumber firstBlackPrisonField  = firstWhitePrisonField + numberOfPrisonFields;
    void new_game();
    void enter_setup_mode();
    void leave_setup_mode();
    void piece_removed(const boardgame::PieceNumber id);
    void piece_selected(const boardgame::PieceNumber id);
    void occupy(const boardgame::FieldNumber id);
    void request_set_current_move_and_branch_start_id(const int move_id);
    void request_cut_off(const int move_id);
    void request_move_list_forward();
    void request_move_list_back();
private:
    std::array<boardgame::PieceNumber, numberOfFields.v> mFields {};
    MuehleConstellation mConstellation;
    const Muehle_Ui_Type mUi;
    const Move_List_Ui mlUi;
    std::unique_ptr<boardgame::Move_List<MuehleConstellation, Move_List_Ui>> move_list {new boardgame::Move_List<MuehleConstellation, Move_List_Ui>(mlUi)};
    struct Player
    {
        const boardgame::Piecegroup<decltype(mConstellation.cbegin())>& mPieceGroup;
        const boardgame::Fieldgroup<decltype(mFields.cbegin())>& mDrawerGroup;
        const boardgame::Fieldgroup<decltype(mFields.cbegin())>& mPrisonGroup;
    };
    void start_move();
    void swap_players();
    bool closed_muehle(const boardgame::FieldNumber fieldNumber) const;
    const std::vector<boardgame::FieldNumber> free_adjacent_fields(const boardgame::FieldNumber fn) const;
    void lock_fields(const boardgame::Fieldgroup<decltype(mFields.cbegin())>& fg) const;
    void occupiable_empty_board_fields();
    void occupiable_empty_fields();
    void set_field_helper(const boardgame::PieceNumber pn, const boardgame::FieldNumber fn);
    void update_game();
    const boardgame::Piecegroup<decltype(mConstellation.cbegin())> mWhitePieces { make_piecegroup(mConstellation, firstWhitePiece, numberOfPiecesPerPlayer) };
    const boardgame::Piecegroup<decltype(mConstellation.cbegin())> mBlackPieces { make_piecegroup(mConstellation, firstBlackPiece, numberOfPiecesPerPlayer) };
    const boardgame::Fieldgroup<decltype(mFields.cbegin())> mBoard { make_fieldgroup(mFields, firstBoardField, numberOfBoardFields) };
    const boardgame::Fieldgroup<decltype(mFields.cbegin())> mWhiteDrawer { make_fieldgroup(mFields, firstWhiteDrawerField, numberOfDrawerFields) };
    const boardgame::Fieldgroup<decltype(mFields.cbegin())> mBlackDrawer { make_fieldgroup(mFields, firstBlackDrawerField, numberOfDrawerFields) };
    const boardgame::Fieldgroup<decltype(mFields.cbegin())> mWhitePrison { make_fieldgroup(mFields, firstWhitePrisonField, numberOfPrisonFields) };
    const boardgame::Fieldgroup<decltype(mFields.cbegin())> mBlackPrison { make_fieldgroup(mFields, firstBlackPrisonField, numberOfPrisonFields) };
    const Player mWhite {mWhitePieces, mWhiteDrawer, mWhitePrison};
    const Player mBlack {mBlackPieces, mBlackDrawer, mBlackPrison};
    const Player* mCurrent {&mWhite};
    const Player* mOpponent {&mBlack};
    boardgame::PieceNumber mSelectedPiece {};
    std::vector<boardgame::PieceNumber> mSelectablePieces {};
    std::vector<boardgame::FieldNumber> mOccupiableFields {};
    bool mSetupMode {false};
    const std::vector<std::vector<int>> mAdjacentFields {
        {1, 9},
        {0, 2, 4},
        {1, 14},
        {4, 10},
        {1, 3, 5, 7},
        {4, 13},
        {7, 11},
        {4, 6, 8},
        {7, 12},
        {0, 10, 21},
        {3, 9, 11, 18},
        {6, 10, 15},
        {8, 13, 17},
        {5, 12, 14, 20},
        {2, 13, 23},
        {11, 16},
        {15, 17, 19},
        {12, 16},
        {10, 19},
        {16, 18, 20, 22},
        {13, 19},
        {9, 22},
        {19, 21, 23},
        {14, 22}
    };
    static constexpr std::array<std::array<int, 2>, 24> mVerticalMuehle {{
        {9, 21},
        {4, 7},
        {14, 23},
        {10, 18},
        {1, 7},
        {13, 20},
        {11, 15},
        {1, 4},
        {12, 17},
        {0, 21},
        {3, 18},
        {6, 15},
        {8, 17},
        {5, 20},
        {2, 23},
        {6, 11},
        {19, 22},
        {8, 12},
        {3, 10},
        {16, 22},
        {5,13},
        {0, 9},
        {16, 19},
        {2, 14}
    }};
};

#endif // SRC_MUEHLE_MUEHLE
