#ifndef TESTS_BOARDGAME_MOVE_LIST_TEST
#define TESTS_BOARDGAME_MOVE_LIST_TEST

#include "boardgame/move_list.h"
#include "boardgame/move_list_ui.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <memory>

class Move_List_Ui_Mock
{
public:
    MOCK_METHOD1(initial_constellation, void(const int));
    MOCK_METHOD1(current_move, void(const int));
    MOCK_METHOD4(add_move, void(const int, const int, const std::string&, const std::vector<int>&));
    MOCK_METHOD1(delete_move, void(const int));
    MOCK_METHOD1(cut_off, void(const int));
    MOCK_METHOD1(need_confirm, void(bool));
};

class Move_List_Test
        : public ::testing::Test
{
protected:
    Move_List_Test();
    void commit_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id);
    void expect_present_move(int move_id, int predecessor);
    void expect_import_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id);
    ::testing::StrictMock<Move_List_Ui_Mock> ui {};
    std::unique_ptr<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>> move_list {};
};

void initial_constellation(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int constellation_id) { mui->initial_constellation(constellation_id); }
void current_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->current_move(move_id); }
void add_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id, const int branch_id, const std::string& description, const std::vector<int>& hint) { mui->add_move(move_id, branch_id, description, hint); }
void delete_move(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->delete_move(move_id); }
void cut_off(::testing::StrictMock<Move_List_Ui_Mock>* mui, const int move_id) { mui->cut_off(move_id); }
void need_confirm(::testing::StrictMock<Move_List_Ui_Mock>* mui, const bool is_needed) { mui->need_confirm(is_needed); }

#endif // TESTS_BOARDGAME_MOVE_LIST_TEST
