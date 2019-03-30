#include "move_list_qml.test.h"

#include <chrono>
#include <thread>

#include <QGuiApplication>
#include <QQmlProperty>

using testing::_;
using testing::Invoke;

void processEvents()
{
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

Move_List_Qml_Test::Move_List_Qml_Test()
    : move_list_component(v.engine(), QUrl(QStringLiteral("qrc:/Move_List_Qml_Test.qml")))
    , move_list_item(qobject_cast<QQuickItem*>(move_list_component.create()))
    , move_list_root_entry(QQmlProperty(move_list_item.get(), "move_list_1").read().value<QQuickItem*>())
    , move_list_root_buttons(QQmlProperty(move_list_root_entry, "buttons").read().value<QQuickItem*>())
    , move_list_control(QQmlProperty(move_list_root_entry, "control").read().value<QQuickItem*>())
    , move_list_back_to_start_button(move_list_control->childItems().at(0))
    , move_list_back_button(move_list_control->childItems().at(1))
    , move_list_forward_button(move_list_control->childItems().at(2))
    , mlq(v.engine(), move_list_root_entry)
{
    Q_INIT_RESOURCE(boardgame_qml);
    Q_INIT_RESOURCE(move_list_qml_test);
    move_list_item->setParentItem(v.contentItem());

    connect(&mlq, &boardgame_qml::Move_List_Qml::request_set_current_move_and_branch_start_id, &move_list_slots, &Move_List_Qml_Test_Slots::request_set_current_move_and_branch_start_id);
    auto set_current_move = [this](int move_id) { mlq.current_move(move_id); };
    ON_CALL(move_list_slots, request_set_current_move_and_branch_start_id(_)).WillByDefault(Invoke(set_current_move));
    connect(&mlq, &boardgame_qml::Move_List_Qml::request_move_list_forward, &move_list_slots, &Move_List_Qml_Test_Slots::request_move_list_forward);
    connect(&mlq, &boardgame_qml::Move_List_Qml::request_move_list_back, &move_list_slots, &Move_List_Qml_Test_Slots::request_move_list_back);
    mlq.initial_constellation(0);
    processEvents();
}

std::vector<QQuickItem*> Move_List_Qml_Test::add_sequence(int current_move_id, int move_id, int number_of_moves, int expected_number_of_childItems)
{
    mlq.current_move(current_move_id);
    auto end = move_id + number_of_moves;
    auto entries = mlq.add_move(move_id, std::to_string(move_id));
    if (expected_number_of_childItems == 0) {
        EXPECT_TRUE(entries.empty());
    } else {
        auto parentbuttons = QQmlProperty(entries.at(0)->parentItem(), "buttons").read().value<QQuickItem*>();
        EXPECT_EQ(expected_number_of_childItems, entries.at(0)->parentItem()->childItems().size());
        for (auto button : parentbuttons->childItems()) {
            EXPECT_TRUE(QQmlProperty(button, "move_id").read().value<int>() <= current_move_id);
        }
        for (unsigned i = 1; i < entries.size(); i += 1) {
            EXPECT_EQ(parentbuttons, QQmlProperty(entries.at(i)->parentItem(), "buttons").read().value<QQuickItem*>());
        }
    }
    move_id += 1;
    for (; move_id < end; move_id += 1) {
        auto noentries = mlq.add_move(move_id, std::to_string(move_id));
        EXPECT_TRUE(noentries.empty());
    }
    return entries;
}

TEST_F(Move_List_Qml_Test, controls)
{
    auto entries = mlq.add_move(1, "01");
    EXPECT_EQ(0, entries.size());
    EXPECT_EQ(4, move_list_root_buttons->childItems().length());
    EXPECT_FALSE(QQmlProperty(move_list_root_buttons->childItems().at(1), "visible").read().toBool());
    EXPECT_CALL(move_list_slots, request_set_current_move_and_branch_start_id(0));
    QMetaObject::invokeMethod(move_list_back_to_start_button, "clicked");
    mlq.current_move(0);
    entries = mlq.add_move(2, "02");
    EXPECT_EQ(2, entries.size());
    EXPECT_EQ(move_list_root_entry, entries.at(0)->parentItem());
    EXPECT_EQ(move_list_root_entry, entries.at(1)->parentItem());
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_back());
    QMetaObject::invokeMethod(move_list_back_button, "clicked");
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_forward());
    QMetaObject::invokeMethod(move_list_forward_button, "clicked");
    processEvents();
}

TEST_F(Move_List_Qml_Test, lots_of_moves)
{
    add_sequence(0, 1, 100, 0);
    EXPECT_EQ(103, move_list_root_buttons->childItems().length()); // controls + trash icon + eye icon + 100 move buttons
    auto entries = add_sequence(50, 101, 100, 3);
    EXPECT_EQ(move_list_root_entry, entries.at(0)->parentItem());
    EXPECT_EQ(move_list_root_entry, entries.at(1)->parentItem());
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // only 50 move buttons left
    EXPECT_EQ(52, (QQmlProperty(entries.at(0), "buttons").read().value<QQuickItem*>())->childItems().length()); // 50 move buttons reparented
    EXPECT_EQ(102, (QQmlProperty(entries.at(1), "buttons").read().value<QQuickItem*>())->childItems().length());
    entries = add_sequence(50, 201, 100, 4); // only one entry returned, because 50 is already the last childItem of move_list_root_buttons
    EXPECT_EQ(move_list_root_entry, entries.at(0)->parentItem());
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // still 50 move buttons left
    EXPECT_EQ(102, (QQmlProperty(entries.at(0), "buttons").read().value<QQuickItem*>())->childItems().length());
    add_sequence(50, 301, 100, 5);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length());
    entries = add_sequence(100, 401, 100, 0); // continue after move 100: ... -> 98 -> 99 -> 100 -> 401 -> 402 -> 403 -> ...
    EXPECT_EQ(152, (QQmlProperty(move_list_root_entry->childItems().at(1), "buttons").read().value<QQuickItem*>())->childItems().length()); // 51 - 500

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

    entries = add_sequence(25, 501, 100, 3);
    EXPECT_EQ(28, move_list_root_buttons->childItems().length());

    /* content:
     * +---+   +---+   +---+          +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 24+-->+ 25|
     * +---+   +---+   +---+          +---+   +---+
     *                                          |
     *                                          |
     *                                          |     +---+   +---+   +---+          +---+   +---+
     *                                          +---->+  0+-->+  1+-->+  2+-->...+-->+ 24+-->+ 25|
     *                                          |     +---+   +---+   +---+          +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+          +---+   +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              +---->+ 51+-->...+-->+ 98+-->+ 99+-->+100+-->+401+-->...+-->+498+-->+499+-->+500|
     *                                          |                                              |     +---+          +---+   +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              +---->+101+-->+102+-->+103+-->...+-->+198+-->+199|-->+200|
     *                                          |                                              |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              +---->+201+-->+202+-->+203+-->...+-->+298+-->+299|-->+300|
     *                                          |                                              |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              |
     *                                          |                                              |     +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |                                              +---->+301+-->+302+-->+303+-->...+-->+398+-->+399|-->+400|
     *                                          |                                                    +---+   +---+   +---+          +---+   +---+   +---+
     *                                          |
     *                                          |     +---+   +---+          +---+   +---+   +---+
     *                                          +---->+501+-->+502+-->...+-->+598+-->+599+-->+600|
     *                                                +---+   +---+          +---+   +---+   +---+
     */
}

TEST_F(Move_List_Qml_Test, delete_and_cut_off)
{
    add_sequence(0, 1, 100, 0);
    add_sequence(50, 101, 100, 3);
    add_sequence(150, 201, 100, 3);
    add_sequence(250, 301, 100, 3);

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

    mlq.delete_move(200);
    mlq.delete_move(199);
    mlq.delete_move(198);

    auto entries101to150 = move_list_root_entry->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries151to197 = entries101to150->childItems().at(1);
    EXPECT_EQ(49, QQmlProperty(entries151to197, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries201to250 = entries101to150->childItems().at(2);
    EXPECT_EQ(52, QQmlProperty(entries201to250, "buttons").read().value<QQuickItem*>()->childItems().length());

    auto entries301to400 = entries201to250->childItems().at(2);
    EXPECT_EQ(102, QQmlProperty(entries301to400, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 200; i > 150; i -= 1) {
        mlq.delete_move(i);
    }
    EXPECT_EQ(2, QQmlProperty(entries151to197, "buttons").read().value<QQuickItem*>()->childItems().length());

    mlq.cut_off(151); // 201 - 250 move behind 150
    EXPECT_EQ(102, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 400; i > 300; i -= 1) {
        mlq.delete_move(i);
    }
    EXPECT_EQ(2, QQmlProperty(entries301to400, "buttons").read().value<QQuickItem*>()->childItems().length());

    mlq.cut_off(301); // 251 - 300 move behind 250, which is behind 101 now
    EXPECT_EQ(152, QQmlProperty(entries101to150, "buttons").read().value<QQuickItem*>()->childItems().length());

    for (int i = 100; i > 50; i -= 1) {
        mlq.delete_move(i);
    }
    EXPECT_EQ(2, QQmlProperty(move_list_root_entry->childItems().at(1), "buttons").read().value<QQuickItem*>()->childItems().length());

    mlq.cut_off(51); // 101 - 300 move behind 50
    EXPECT_EQ(203, move_list_root_buttons->childItems().length()); // one more because of controls, so 203
}

int main(int argc, char **argv)
{
    QGuiApplication app {argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
