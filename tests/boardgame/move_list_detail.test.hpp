#ifndef TESTS_BOARDGAME_MOVE_LIST_DETAIL_TEST_HPP
#define TESTS_BOARDGAME_MOVE_LIST_DETAIL_TEST_HPP

#include "boardgame/move_list.hpp"

#include "gtest/gtest.h"

class Move_List_Detail_Test
        : public ::testing::Test
{
protected:
    void commit_sequence(int predecessor, int start, int end, int expected_committed_move_id);
    void expect_present_constellation(int content, int predecessor);
    void delete_moves(int move_id);
    boardgame::detail::Move_List<int> move_list {};
};

#endif // TESTS_BOARDGAME_MOVE_LIST_DETAIL_TEST_HPP
