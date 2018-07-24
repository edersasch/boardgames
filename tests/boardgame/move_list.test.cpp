#include "move_list.test.h"

void Move_List_Test::commit_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id)
{
    if (predecessor != -1) {
        EXPECT_CALL(ui, current_move(predecessor));
        move_list.set_current_move_id(predecessor);
    } else {
        EXPECT_CALL(ui, initial_constellation(0));
    }
    for (int i = start; i <= end; i += 1, expected_committed_move_id += 1) {
        if (predecessor != -1 || i != start) { // otherwise "initial_constellation(0)" above gets called
            EXPECT_CALL(ui, add_move(expected_committed_move_id, expected_branch_start_id, std::to_string(i), std::vector<int>{}));
        }
        EXPECT_CALL(ui, current_move(expected_committed_move_id));
        move_list.commit(i, std::to_string(i), {});
        EXPECT_EQ(expected_committed_move_id, move_list.get_current_move_id());
        EXPECT_EQ(std::vector<int>(), move_list.hint());
    }
    EXPECT_EQ(expected_branch_start_id, move_list.get_current_branch_start_id());
}

void Move_List_Test::expect_present_move(int move_id, int predecessor)
{
    EXPECT_CALL(ui, current_move(predecessor));
    move_list.set_current_move_id(predecessor);
    EXPECT_CALL(ui, current_move(move_id));
    move_list.commit(move_id, std::to_string(move_id), {});
}

TEST_F(Move_List_Test, commit_sequence_test)
{
    commit_sequence(-1, 0, 99, 0, 0);
    EXPECT_FALSE(move_list.move_list_forward());
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(150, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);

    EXPECT_FALSE(move_list.set_current_move_and_branch_start_id(14000)); // no such move id

    EXPECT_CALL(ui, current_move(300));
    move_list.set_current_move_id(300);
    EXPECT_CALL(ui, current_move(250));
    move_list.move_list_back();
    EXPECT_EQ(250, move_list.get_current_move_id());
    EXPECT_EQ(300, move_list.get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(300));
    move_list.move_list_forward();
    EXPECT_EQ(300, move_list.get_current_move_id());
    EXPECT_EQ(300, move_list.get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(150));
    move_list.set_current_move_and_branch_start_id(150);
    EXPECT_EQ(150, move_list.get_current_move_id());
    EXPECT_EQ(100, move_list.get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(149));
    move_list.move_list_back();
    EXPECT_EQ(149, move_list.get_current_move_id());
    EXPECT_EQ(100, move_list.get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(150));
    move_list.move_list_forward();
    EXPECT_EQ(150, move_list.get_current_move_id());
    EXPECT_EQ(100, move_list.get_current_branch_start_id());
    EXPECT_CALL(ui, current_move(151));
    move_list.move_list_forward();
    EXPECT_EQ(151, move_list.get_current_move_id());
    EXPECT_EQ(100, move_list.get_current_branch_start_id());

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

TEST_F(Move_List_Test, cut_off_test)
{
    commit_sequence(-1, 0, 99, 0, 0);
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

    for (int i = 100; i < 200; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    for (int i = 400; i < 500; i += 1) {
        EXPECT_CALL(ui, delete_move(i));
    }
    EXPECT_CALL(ui, cut_off(100));
    EXPECT_CALL(ui, current_move(0)); // current_move_id 499 gets deleted, so current_constellation is set to 0
    move_list.cut_off(100);

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
    move_list.cut_off(251);
    EXPECT_CALL(ui, current_move(250));
    move_list.set_current_move_and_branch_start_id(250);
    commit_sequence(250, 500, 599, 500, 500);
    EXPECT_CALL(ui, current_move(250));
    move_list.set_current_move_and_branch_start_id(250);
    EXPECT_CALL(ui, current_move(300));
    move_list.move_list_forward();
}

TEST_F(Move_List_Test, detect_present_constellation)
{
    commit_sequence(-1, 0, 99, 0, 0);
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
    move_list.cut_off(400);
    move_list.cut_off(400); // already gone

    commit_sequence(150, 400, 499, 500, 500);

    // move_list content looks the same as before, but consellations 400 onwards have move ids of 500 onwards
}
