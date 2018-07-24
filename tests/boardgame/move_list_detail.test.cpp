#include "move_list_detail.test.h"

void Move_List_Detail_Test::commit_sequence(int predecessor, int start, int end, int expected_committed_move_id, int expected_branch_start_id)
{
    int branch_start_id = 0;
    for (int i = start; i <= end; i += 1, expected_committed_move_id += 1) {
        int old = predecessor;
        std::tie(predecessor, branch_start_id) = move_list.commit(i, predecessor);
        EXPECT_EQ(expected_committed_move_id, predecessor);
        EXPECT_EQ(old, move_list.prev(expected_committed_move_id));
        EXPECT_EQ(i, move_list.constellation(predecessor));
        if (move_list.has_entry(old)) {
            EXPECT_TRUE(std::find(move_list.next(old).begin(), move_list.next(old).end(), predecessor) != move_list.next(old).end());
            if (i > start) {
                EXPECT_EQ(1U, move_list.next(old).size());
            }
        }
        EXPECT_TRUE(move_list.has_entry(expected_committed_move_id));
        EXPECT_EQ(expected_branch_start_id, branch_start_id);
    }
}

void Move_List_Detail_Test::expect_present_constellation(int content, int predecessor)
{
    auto [move_id, branch_start_id] = move_list.commit(content, predecessor);
    EXPECT_EQ(content, move_id);
    EXPECT_EQ(-1, branch_start_id);
}

void Move_List_Detail_Test::delete_moves(int move_id)
{
    for (int id : move_list.next(move_id)) { // go to the end of each branch and start deleting there
        delete_moves(id);
    }
    move_list.delete_move(move_id);
}

TEST_F(Move_List_Detail_Test, commit_sequence_test)
{
    commit_sequence(-1, 0, 99, 0, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(150, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);

    EXPECT_EQ(2U, move_list.next(50).size());
    EXPECT_EQ(2U, move_list.next(150).size());
    EXPECT_EQ(2U, move_list.next(250).size());
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(50));
    EXPECT_EQ((std::vector<int>{0}), move_list.branch_start_ids(51));

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
    EXPECT_EQ(1U, move_list.next(99).size());

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

TEST_F(Move_List_Detail_Test, delete_moves_test)
{
    commit_sequence(-1, 0, 99, 0, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(50, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);
    commit_sequence(150, 400, 499, 400, 400);

    EXPECT_EQ(3U, move_list.next(50).size());
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(50));
    EXPECT_EQ((std::vector<int>{100, 400}), move_list.branch_start_ids(100));
    EXPECT_EQ((std::vector<int>{100, 400}), move_list.branch_start_ids(101));
    EXPECT_EQ((std::vector<int>{100, 400}), move_list.branch_start_ids(150));
    EXPECT_EQ((std::vector<int>{100}), move_list.branch_start_ids(151));

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

    delete_moves(100);
    EXPECT_EQ(2U, move_list.next(50).size());
    EXPECT_FALSE(move_list.has_entry(100));
    EXPECT_FALSE(move_list.has_entry(150));
    EXPECT_FALSE(move_list.has_entry(199));
    EXPECT_FALSE(move_list.has_entry(400));
    EXPECT_FALSE(move_list.has_entry(450));
    EXPECT_FALSE(move_list.has_entry(499));
    EXPECT_EQ((std::vector<int>{0, 200, 300}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 200, 300}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 200, 300}), move_list.branch_start_ids(50));

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
}

TEST_F(Move_List_Detail_Test, detect_present_constellation)
{
    commit_sequence(-1, 0, 99, 0, 0);
    commit_sequence(50, 100, 199, 100, 100);
    commit_sequence(50, 200, 299, 200, 200);
    commit_sequence(250, 300, 399, 300, 300);
    commit_sequence(150, 400, 499, 400, 400);

    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 400}), move_list.branch_start_ids(50));
    EXPECT_EQ((std::vector<int>{0}), move_list.branch_start_ids(51));
    EXPECT_EQ((std::vector<int>{100, 400}), move_list.branch_start_ids(123));

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

    expect_present_constellation(51, 50);
    expect_present_constellation(100, 50);
    expect_present_constellation(200, 50);
    expect_present_constellation(400, 150);
    expect_present_constellation(300, 250);

    delete_moves(400);

    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300}), move_list.branch_start_ids(50));
    EXPECT_EQ((std::vector<int>{100}), move_list.branch_start_ids(123));

    commit_sequence(150, 400, 499, 500, 500);

    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 500}), move_list.branch_start_ids(0));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 500}), move_list.branch_start_ids(1));
    EXPECT_EQ((std::vector<int>{0, 100, 200, 300, 500}), move_list.branch_start_ids(50));
    EXPECT_EQ((std::vector<int>{100, 500}), move_list.branch_start_ids(123));

    EXPECT_EQ(move_list.constellation(500), 400);
    EXPECT_EQ(move_list.next(150).at(1), 500);

    // move_list content looks the same as before, but consellations 400 onwards have move ids of 500 onwards
}
