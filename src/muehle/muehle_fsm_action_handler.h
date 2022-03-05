#ifndef SRC_MUEHLE_MUEHLE_FSM_ACTION_HANDLER
#define SRC_MUEHLE_MUEHLE_FSM_ACTION_HANDLER

#include "boardgame/pieces_n_fields.h"

namespace muehle
{

class Muehle_FSM_Action_Handler
{
public:
    virtual void start_new_game() = 0;
    virtual void stop_engine() = 0;
    virtual void enter_setup_mode() = 0;
    virtual void exit_setup_mode() = 0;
    virtual void set_selectable_game_pieces() = 0;
    virtual void select_game_piece(boardgame::Piece_Number pn) = 0;
    virtual void select_setup_piece(boardgame::Piece_Number pn) = 0;
    virtual void occupy_setup(boardgame::Field_Number fn) = 0;
    virtual void occupy_game(boardgame::Field_Number fn) = 0;
    virtual void remove_piece_and_finish_move(boardgame::Piece_Number pn) = 0;
    virtual void finish_move() = 0;
    virtual void finish_engine_move() = 0;
    virtual void import_move_list() = 0;
    virtual void set_move_and_branch() = 0;
    virtual void restore_last_constellation() = 0;
};

}

#endif // SRC_MUEHLE_MUEHLE_FSM_ACTION_HANDLER
