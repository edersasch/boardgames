#ifndef TESTS_MUEHLE_MUEHLE_FSM_TEST
#define TESTS_MUEHLE_MUEHLE_FSM_TEST

#include "muehle/muehle_fsm_action_handler.h"
#include "boardgame/pieces_n_fields.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class Muehle_FSM_Action_Handler_Mock
        : public muehle::Muehle_FSM_Action_Handler
{
public:
    MOCK_METHOD0(start_new_game, void());
    MOCK_METHOD0(stop_engine, void());
    MOCK_METHOD0(enter_setup_mode, void());
    MOCK_METHOD0(exit_setup_mode, void());
    MOCK_METHOD0(set_selectable_game_pieces, void());
    MOCK_METHOD1(select_game_piece, void(boardgame::Piece_Number));
    MOCK_METHOD1(select_setup_piece, void(boardgame::Piece_Number));
    MOCK_METHOD1(occupy_setup, void(boardgame::Field_Number));
    MOCK_METHOD1(occupy_game, void(boardgame::Field_Number));
    MOCK_METHOD1(remove_piece_and_finish_move, void(boardgame::Piece_Number));
    MOCK_METHOD0(finish_move, void());
    MOCK_METHOD0(finish_engine_move, void());
    MOCK_METHOD0(import_move_list, void());
    MOCK_METHOD0(set_move_and_branch, void());
    MOCK_METHOD0(restore_last_constellation, void());
};

class Muehle_FSM_Test
        : public ::testing::Test
{
protected:
    Muehle_FSM_Test();
    ~Muehle_FSM_Test() = default;
    ::testing::StrictMock<Muehle_FSM_Action_Handler_Mock> mAction_Handler_Mock {};
};

# endif // TESTS_MUEHLE_MUEHLE_FSM_TEST
