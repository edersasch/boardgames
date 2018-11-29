#include "multi_move_list_qml.test.h"

#include <chrono>
#include <thread>

#include <QGuiApplication>
#include <QQmlProperty>

using testing::_;
using testing::Invoke;

Multi_Move_List_Qml_Test::Multi_Move_List_Qml_Test()
    : move_list_component(v.engine(), QUrl(QStringLiteral("qrc:/Move_List_Qml_Test.qml")))
    , move_list_item(qobject_cast<QQuickItem*>(move_list_component.create()))
    , move_list_root_entry1(QQmlProperty(move_list_item.get(), "move_list_1").read().value<QQuickItem*>())
    , move_list_root_entry2(QQmlProperty(move_list_item.get(), "move_list_2").read().value<QQuickItem*>())
    , move_list_root_buttons(QQmlProperty(move_list_root_entry1, "buttons").read().value<QQuickItem*>())
    , move_list_control(QQmlProperty(move_list_root_entry1, "control").read().value<QQuickItem*>())
    , move_list_back_to_start_button(move_list_control->childItems().at(0))
    , move_list_back_button(move_list_control->childItems().at(1))
    , move_list_forward_button(move_list_control->childItems().at(2))
    , mlq(v.engine(), {move_list_root_entry1, move_list_root_entry2}, "test", "tst")
{
    Q_INIT_RESOURCE(boardgame_qml);
    Q_INIT_RESOURCE(move_list_qml_test);
    move_list_item->setParentItem(v.contentItem());

    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_set_current_move_and_branch_start_id, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_set_current_move_and_branch_start_id);
    auto set_current_move = [this](const int move_id) { current_move(&mlq, move_id); };
    ON_CALL(move_list_slots, request_set_current_move_and_branch_start_id(_)).WillByDefault(Invoke(set_current_move));
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_forward, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_forward);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_back, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_back);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::added_move, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::added_move);
    auto set_move_color = [this](const int move_id, const int piece_id) { mlq.set_move_color(move_id, piece_id % 2 ? "odd" : "even"); };
    ON_CALL(move_list_slots, added_move(_, _)).WillByDefault(Invoke(set_move_color));
    initial_constellation(&mlq, 0);
    processEvents();
}

void Multi_Move_List_Qml_Test::add_sequence(int move_id, int branch_start_id, int number_of_moves)
{
    auto end = move_id + number_of_moves;
    for (; move_id < end; move_id += 1) {
        EXPECT_CALL(move_list_slots, added_move(move_id, -1));
        add_move(&mlq, move_id, branch_start_id, std::to_string(move_id), {});
    }
}

TEST_F(Multi_Move_List_Qml_Test, controls)
{
    EXPECT_CALL(move_list_slots, added_move(1, -1));
    add_move(&mlq, 1, 0, "01", {});
    EXPECT_CALL(move_list_slots, request_set_current_move_and_branch_start_id(0));
    QMetaObject::invokeMethod(move_list_back_to_start_button, "clicked");
    current_move(&mlq, 0);
    EXPECT_CALL(move_list_slots, added_move(2, 5));
    add_move(&mlq, 2, 2, "02", {5});
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_back());
    QMetaObject::invokeMethod(move_list_back_button, "clicked");
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_forward());
    QMetaObject::invokeMethod(move_list_forward_button, "clicked");
    processEvents();
}

TEST_F(Multi_Move_List_Qml_Test, lots_of_moves)
{
    add_sequence(1, 0, 100);
    current_move(&mlq, 50);
    EXPECT_EQ(103, move_list_root_buttons->childItems().length()); // controls + trash icon + eye icon + 100 move buttons
    add_sequence(101, 101, 100);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // only 50 move buttons left
    current_move(&mlq, 50);
    add_sequence(201, 201, 100);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // still 50 move buttons left
    current_move(&mlq, 50);
    add_sequence(301, 0, 100); // continue after move 50: ... -> 48 -> 49 -> 50 -> 301 -> 302 -> 303 -> ...
    EXPECT_EQ(153, move_list_root_buttons->childItems().length());
    current_move(&mlq, 100);
    add_sequence(401, 0, 100); // continue after move 100: ... -> 98 -> 99 -> 100 -> 401 -> 402 -> 403 -> ...
    EXPECT_EQ(152, (QQmlProperty(move_list_root_entry1->childItems().at(1), "buttons").read().value<QQuickItem*>())->childItems().length()); // 51 - 500

    /* content:
     * +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50|
     * +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |     +---+          +---+   +---+   +---+   +---+          +---+   +---+   +---+
     *                                          +---->+ 51+-->...+-->+ 98+-->+ 99+-->+100+-->+401+-->...+-->+498+-->+499+-->+500|
     *                                          |     +---+          +---+   +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          +---->+101+-->+102+-->+103+-->...+-->+198+-->+199|-->+200|
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          +---->+201+-->+202+-->+203+-->...+-->+298+-->+299|-->+300|
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          +---->+301+-->+302+-->+303+-->...+-->+398+-->+399|-->+400|
     *                                                +---+   +---+   +---+          +---+   +---+   +---+
     */
}

TEST_F(Multi_Move_List_Qml_Test, delete_and_cut_off)
{
    add_sequence(1, 0, 100);
    current_move(&mlq, 50);
    add_sequence(101, 101, 100);
    current_move(&mlq, 150);
    add_sequence(201, 201, 100);
    current_move(&mlq, 250);
    add_sequence(301, 301, 100);

   /*
    * +---+   +---+   +---+          +---+   +---+
    * |  0+-->+  1+-->+  2+-->...+-->+ 49+-->+ 50|
    * +---+   +---+   +---+          +---+   +---+
    *                                          |
    *                                          |     +---+          +---+   +---+   +---+
    *                                          +---->+ 51+-->...+-->+ 98+-->+ 99+-->+100|
    *                                          |     +---+          +---+   +---+   +---+
    *                                          |
    *                                          |     +---+   +---+   +---+         +---+   +---+
    *                                          +---->+101+-->+102+-->+103+-->...-->+149+-->+150+
    *                                                +---+   +---+   +---+         +---+   +---+
    *                                                                                        |
    *                                                                                        |     +---+          +---+   +---+   +---+
    *                                                                                        +---->+151+-->...+-->+198+-->+199+-->+200|
    *                                                                                        |     +---+          +---+   +---+   +---+
    *                                                                                        |
    *                                                                                        |     +---+   +---+   +---+          +---+   +---+
    *                                                                                        +---->+201+-->+202+-->+203+-->...+-->+249+-->+250+
    *                                                                                              +---+   +---+   +---+          +---+   +---+
    *                                                                                                                                       |
    *                                                                                                                                       |     +---+          +---+   +---+   +---+
    *                                                                                                                                       +---->+251+-->...+-->+298+-->+299+-->+300|
    *                                                                                                                                       |     +---+          +---+   +---+   +---+
    *                                                                                                                                       |
    *                                                                                                                                       |     +---+          +---+   +---+   +---+
    *                                                                                                                                       +---->+301+-->...+-->+398+-->+399|-->+400|
    *                                                                                                                                             +---+          +---+   +---+   +---+
    */

    delete_move(&mlq, 200);
    delete_move(&mlq, 199);
    delete_move(&mlq, 198);

    auto entries101to150 = move_list_root_entry1->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries151to197 = entries101to150->childItems().at(1);
    EXPECT_EQ(49, QQmlProperty(entries151to197, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries201to250 = entries101to150->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries201to250, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries301to400 = entries201to250->childItems().at(2);
    EXPECT_EQ(102, QQmlProperty(entries301to400, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries101to150_2 = move_list_root_entry2->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries101to150_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries151to197_2 = entries101to150_2->childItems().at(1);
    EXPECT_EQ(49, QQmlProperty(entries151to197_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries201to250_2 = entries101to150_2->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries201to250_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries301to400_2 = entries201to250_2->childItems().at(2);
    EXPECT_EQ(102, QQmlProperty(entries301to400_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 200; i > 150; i -= 1) {
        delete_move(&mlq, i);
    }
    EXPECT_EQ(2, QQmlProperty(entries151to197, "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_EQ(2, QQmlProperty(entries151to197_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    cut_off(&mlq, 151); // 201 - 250 move behind 150
    EXPECT_EQ(102, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_EQ(102, QQmlProperty(entries101to150_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 400; i > 300; i -= 1) {
        delete_move(&mlq, i);
    }
    EXPECT_EQ(2, QQmlProperty(entries301to400, "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_EQ(2, QQmlProperty(entries301to400_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    cut_off(&mlq, 301); // 251 - 300 move behind 250, which is behind 101 now
    EXPECT_EQ(152, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_EQ(152, QQmlProperty(entries101to150_2, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 100; i > 50; i -= 1) {
        delete_move(&mlq, i);
    }
    EXPECT_EQ(2, QQmlProperty(move_list_root_entry1->childItems().at(1), "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_EQ(2, QQmlProperty(move_list_root_entry2->childItems().at(1), "buttons").read().value<QQuickItem*>()->childItems().length());

    cut_off(&mlq, 51); // 101 - 300 move behind 50
    EXPECT_EQ(203, move_list_root_buttons->childItems().length()); // one more because of controls, so 203
}
