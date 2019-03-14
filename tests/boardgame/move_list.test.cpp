#include "move_list.test.h"

#include <fstream>

namespace
{

std::string diff_text(std::vector<std::array<int, 2>> d)
{
    return std::to_string(d.front().at(1));
}

}

Move_List_Test::Move_List_Test()
{
    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, std::vector<int>{0}, std::vector<int>{0});
}

void Move_List_Test::commit_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id)
{
    EXPECT_CALL(ui, current_move(predecessor));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(predecessor));
    for (int i = start; i <= end; i += 1, expected_committed_move_id += 1) {
        EXPECT_CALL(ui, add_move(expected_committed_move_id, expected_branch_start_id, std::to_string(i), std::vector<int>{}));
        EXPECT_CALL(ui, current_move(expected_committed_move_id));
        EXPECT_TRUE(move_list->commit({i}, {}));
        EXPECT_EQ(expected_committed_move_id, move_list->get_current_move_id());
        EXPECT_EQ(std::vector<int>(), move_list->hint());
    }
    EXPECT_EQ(expected_branch_start_id, move_list->get_current_branch_start_id());
}

void Move_List_Test::expect_present_move(int move_id, int predecessor)
{
    EXPECT_CALL(ui, current_move(predecessor));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(predecessor));
    EXPECT_CALL(ui, current_move(move_id));
    EXPECT_TRUE(move_list->commit({move_id}, {}));
}

void Move_List_Test::expect_import_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id)
{
    EXPECT_CALL(ui, current_move(predecessor)).RetiresOnSaturation();
    for (int i = start; i <= end; i += 1, expected_committed_move_id += 1) {
        if (i != end) {
            EXPECT_CALL(ui, current_move(expected_committed_move_id));
        }
        EXPECT_CALL(ui, add_move(expected_committed_move_id, expected_branch_start_id, std::to_string(i), std::vector<int>{}));
    }
}

TEST_F(Move_List_Test, commit_sequence)
{
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 99, 1, 0);
    EXPECT_FALSE(move_list->move_list_forward());
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(150, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);

    EXPECT_FALSE(move_list->set_current_move_and_branch_start_id(14000)); // no such move id

    EXPECT_CALL(ui, current_move(300));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(300));
    EXPECT_CALL(ui, current_move(250));
    EXPECT_TRUE(move_list->move_list_back());
    EXPECT_EQ(250, move_list->get_current_move_id());
    EXPECT_EQ(300, move_list->get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(300));
    EXPECT_TRUE(move_list->move_list_forward());
    EXPECT_EQ(300, move_list->get_current_move_id());
    EXPECT_EQ(300, move_list->get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(150));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(150));
    EXPECT_EQ(150, move_list->get_current_move_id());
    EXPECT_EQ(100, move_list->get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(149));
    EXPECT_TRUE(move_list->move_list_back());
    EXPECT_EQ(149, move_list->get_current_move_id());
    EXPECT_EQ(100, move_list->get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(150));
    EXPECT_TRUE(move_list->move_list_forward());
    EXPECT_EQ(150, move_list->get_current_move_id());
    EXPECT_EQ(100, move_list->get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(151));
    EXPECT_TRUE(move_list->move_list_forward());
    EXPECT_EQ(151, move_list->get_current_move_id());
    EXPECT_EQ(100, move_list->get_current_branch_start_id());

    /* move_list content:
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50+-->+ 51+-->...+-->+ 98+-->+ 99|
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+100+-->+101+-->+102+-->...+-->+149+-->+150+-->+151+-->...+-->+198+-->+199|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         |
     *                                                                                         |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         +---->+200+-->+201+-->+202+-->...+-->+249+-->+250+-->+251+-->...+-->+298+-->+299|
     *                                                                                               +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                                                                        |
     *                                                                                                                                        |     +---+   +---+          +---+   +---+
     *                                                                                                                                        +---->+300+-->+301+-->...+-->+398+-->+399|
     *                                                                                                                                              +---+   +---+          +---+   +---+
     */

    commit_sequence(99, 400, 499, 400, 0);

    /* move_list content:
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50+-->+ 51+-->...+-->+ 98+-->+ 99+-->+400+-->+401+-->...+-->+498+-->+499|
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+100+-->+101+-->+102+-->...+-->+149+-->+150+-->+151+-->...+-->+198+-->+199|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         |
     *                                                                                         |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         +---->+200+-->+201+-->+202+-->...+-->+249+-->+250+-->+251+-->...+-->+298+-->+299|
     *                                                                                               +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                                                                        |
     *                                                                                                                                        |     +---+   +---+          +---+   +---+
     *                                                                                                                                        +---->+300+-->+301+-->...+-->+398+-->+399|
     *                                                                                                                                              +---+   +---+          +---+   +---+
     */
}

TEST_F(Move_List_Test, cut_off)
{
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 99, 1, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(50, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);
    commit_sequence(150, 400, 499, 400, 400);

    /* move_list content:
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50+-->+ 51+-->...+-->+ 98+-->+ 99|
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+100+-->+101+-->+102+-->...+-->+149+-->+150+-->+151+-->...+-->+198+-->+199|
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+   +---+          +---+   +---+
     *                                          |                                              +---->+400+-->+401+-->...+-->+498+-->+499|
     *                                          |                                                    +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+200+-->+201+-->+202+-->...+-->+249+-->+250+-->+251+-->...+-->+298+-->+299|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         |
     *                                                                                         |     +---+   +---+          +---+   +---+
     *                                                                                         +---->+300+-->+301+-->...+-->+398+-->+399|
     *                                                                                               +---+   +---+          +---+   +---+
     */

    EXPECT_FALSE(move_list->cut_off(0)); // nothing happens, initial constellation can't be deleted

    for (int i = 100; i < 200; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    for (int i = 400; i < 500; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, cut_off(100));
    EXPECT_CALL(ui, current_move(0)); // current_move_id 499 gets deleted, so current_constellation is set to 0
    EXPECT_TRUE(move_list->cut_off(100));

    /* move_list content:
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50+-->+ 51+-->...+-->+ 98+-->+ 99|
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+200+-->+201+-->+202+-->...+-->+249+-->+250+-->+251+-->...+-->+298+-->+299|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         |
     *                                                                                         |     +---+   +---+          +---+   +---+
     *                                                                                         +---->+300+-->+301+-->...+-->+398+-->+399|
     *                                                                                               +---+   +---+          +---+   +---+
     */

    for (int i = 251; i < 300; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, cut_off(251));
    EXPECT_TRUE(move_list->cut_off(251));
    EXPECT_CALL(ui, current_move(250));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(250));
    commit_sequence(250, 500, 599, 500, 500);
    EXPECT_CALL(ui, current_move(250));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(250));
    EXPECT_CALL(ui, current_move(300));
    EXPECT_TRUE(move_list->move_list_forward());
}

TEST_F(Move_List_Test, detect_present_constellation)
{
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 99, 1, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(50, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);
    commit_sequence(150, 400, 499, 400, 400);

    /* move_list content:
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50+-->+ 51+-->...+-->+ 98+-->+ 99|
     * +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+100+-->+101+-->+102+-->...+-->+149+-->+150+-->+151+-->...+-->+198+-->+199|
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+   +---+          +---+   +---+
     *                                          |                                              +---->+400+-->+401+-->...+-->+498+-->+499|
     *                                          |                                                    +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                          +---->+200+-->+201+-->+202+-->...+-->+249+-->+250+-->+251+-->...+-->+298+-->+299|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+          +---+   +---+
     *                                                                                         |
     *                                                                                         |     +---+   +---+          +---+   +---+
     *                                                                                         +---->+300+-->+301+-->...+-->+398+-->+399|
     *                                                                                               +---+   +---+          +---+   +---+
     */

    expect_present_move(51, 50);
    expect_present_move(100, 50);
    expect_present_move(200, 50);
    expect_present_move(400, 150);
    expect_present_move(300, 250);

    for (int i = 400; i < 500; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, cut_off(400));
    EXPECT_TRUE(move_list->cut_off(400));
    EXPECT_FALSE(move_list->cut_off(400)); // already gone

    commit_sequence(150, 400, 499, 500, 500);

    // move_list content looks the same as before, but consellations 400 onwards have move ids of 500 onwards
}

TEST_F(Move_List_Test, import_export)
{
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 1, 1, 0);
    commit_sequence(0, 2, 3, 2, 2);
    EXPECT_TRUE(move_list->export_move_list("/tmp/Move_List_Test_import_export_1.out"));

    for (int i = 1; i <= 3; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }

    EXPECT_CALL(ui, current_move(0));
    EXPECT_CALL(ui, cut_off(1));
    EXPECT_TRUE(move_list->cut_off(1));
    EXPECT_CALL(ui, cut_off(2));
    EXPECT_TRUE(move_list->cut_off(2));

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, std::vector<int>{0}, std::vector<int>{0});
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 2, 1, 0);
    commit_sequence(1, 3, 5, 3, 3);
    commit_sequence(1, 6, 8, 6, 6);
    commit_sequence(7, 9, 11, 9, 9);
    commit_sequence(6, 12, 14, 12, 12);
    EXPECT_TRUE(move_list->export_move_list("/tmp/Move_List_Test_import_export_2.out"));

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, std::vector<int>{0}, std::vector<int>{0});
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 99, 1, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(50, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);
    commit_sequence(150, 400, 499, 400, 400);
    EXPECT_TRUE(move_list->export_move_list("/tmp/Move_List_Test_import_export_3.out"));

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    expect_import_sequence(0, 1, 1, 1, 0);
    expect_import_sequence(0, 2, 3, 2, 2);
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_import_export_1.out", true);

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    expect_import_sequence(0, 1, 2, 1, 0);
    expect_import_sequence(1, 3, 5, 3, 3);
    expect_import_sequence(1, 6, 8, 6, 6);
    expect_import_sequence(7, 9, 11, 9, 9);
    expect_import_sequence(6, 12, 14, 12, 12);
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_import_export_2.out", true);

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, std::vector<int>{0}, std::vector<int>{0});
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 2, 1, 0);
    commit_sequence(1, 3, 5, 3, 3);
    commit_sequence(1, 6, 8, 6, 6);
    commit_sequence(7, 9, 11, 9, 9);
    commit_sequence(4, 12, 14, 12, 12);
    EXPECT_TRUE(move_list->export_move_list("/tmp/Move_List_Test_import_export_4.out"));

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    expect_import_sequence(0, 1, 99, 1, 0);
    expect_import_sequence(50, 100, 199, 100, 100);
    expect_import_sequence(50, 200, 299, 200, 200);
    expect_import_sequence(250, 300, 399, 300, 300);
    expect_import_sequence(150, 400, 499, 400, 400);
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_import_export_3.out", true);

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    expect_import_sequence(0, 1, 2, 1, 0);
    expect_import_sequence(1, 3, 5, 3, 3);
    expect_import_sequence(1, 6, 8, 6, 6);
    expect_import_sequence(7, 9, 11, 9, 9);
    expect_import_sequence(4, 12, 14, 12, 12);
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_import_export_4.out", true);
    for (int i = 12; i < 15; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, need_confirm(true));
    EXPECT_CALL(ui, cut_off(12));
    EXPECT_TRUE(move_list->cut_off(12));
}

TEST_F(Move_List_Test, import_export_errors)
{
    EXPECT_CALL(ui, initial_constellation(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/this/file/does/not/exist", true);
    EXPECT_EQ(move_list->get_current_move_id(), -1);

    {
        std::ofstream out1("/tmp/Move_List_Test_broken_json_1.out");
        out1 << "{\"notexpected\": \"something\"}";
    }
    EXPECT_CALL(ui, initial_constellation(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_broken_json_1.out", true);
    EXPECT_EQ(move_list->get_current_move_id(), -1);

    {
        std::ofstream out2("/tmp/Move_List_Test_broken_json_2.out");
        out2 << "[[1,{\"bs\":[0],\"co\":[24],\"hi\":[9],\"ne\":[1],\"pr\":-1}]]"; // the first 1 is not the initial id, so it's not a valid move list
    }
    EXPECT_CALL(ui, initial_constellation(0));
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, "/tmp/Move_List_Test_broken_json_2.out", true);
    EXPECT_EQ(move_list->get_current_move_id(), -1);
}

TEST_F(Move_List_Test, import_export_string)
{
    EXPECT_CALL(ui, need_confirm(true));
    commit_sequence(0, 1, 3, 1, 0);
    commit_sequence(2, 4, 6, 4, 4);
    std::string to_import = move_list->get_move_list_string();
    for (int i = 4; i < 7; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, cut_off(4));
    EXPECT_CALL(ui, current_move(0));
    EXPECT_TRUE(move_list->cut_off(4));

    EXPECT_CALL(ui, initial_constellation(0));
    EXPECT_CALL(ui, current_move(0));
    expect_import_sequence(0, 1, 3, 1, 0);
    expect_import_sequence(2, 4, 6, 4, 4);
    move_list = std::make_unique<boardgame::Move_List<std::vector<int>, boardgame::Move_List_Ui>>(boardgame::Move_List_Ui(&ui), diff_text, to_import, false);
    EXPECT_CALL(ui, current_move(3));
    EXPECT_FALSE(move_list->set_current_move_and_branch_start_id(3, 4));
    EXPECT_CALL(ui, current_move(2));
    EXPECT_TRUE(move_list->set_current_move_and_branch_start_id(2, 4));
    EXPECT_CALL(ui, current_move(4));
    EXPECT_TRUE(move_list->move_list_forward());
}
