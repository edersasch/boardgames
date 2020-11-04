#include "multi_move_list_qml.test.h"

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
    , move_list_import_button(move_list_control->childItems().at(3))
    , move_list_export_button(move_list_control->childItems().at(4))
    , mlq(v.engine(), {move_list_root_entry1, move_list_root_entry2}, "test", "tst")
{
    Q_INIT_RESOURCE(boardgame_qml);
    Q_INIT_RESOURCE(move_list_qml_test);
    move_list_item->setParentItem(v.contentItem());

    const char odd_move_number_as_piece_id[] = "#111111";
    const char even_move_number_as_piece_id[] = "#222222";
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_set_current_move_and_branch_start_id, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_set_current_move_and_branch_start_id);
    auto set_current_move = [this](const int move_id) { current_move(&mlq, move_id); };
    ON_CALL(move_list_slots, request_set_current_move_and_branch_start_id(_)).WillByDefault(Invoke(set_current_move));
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_forward, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_forward);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_back, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_back);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::added_move, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::added_move);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_import, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_import);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_move_list_export, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_move_list_export);
    connect(&mlq, &boardgame_qml::Multi_Move_List_Qml::request_delete_branch, &move_list_slots, &Multi_Move_List_Qml_Test_Slots::request_delete_branch);
    auto set_move_color = [this, odd_move_number_as_piece_id, even_move_number_as_piece_id](const int move_id, const int piece_id) { mlq.set_move_color(move_id, piece_id % 2 ? odd_move_number_as_piece_id : even_move_number_as_piece_id); };
    ON_CALL(move_list_slots, added_move(_, _)).WillByDefault(Invoke(set_move_color));
    initial_constellation(&mlq, 0);
    processEvents();
}

void Multi_Move_List_Qml_Test::add_sequence(int move_id, int number_of_moves)
{
    auto end = move_id + number_of_moves;
    for (; move_id < end; move_id += 1) {
        EXPECT_CALL(move_list_slots, added_move(move_id, move_id));
        add_move(&mlq, move_id, std::to_string(move_id), {move_id});
    }
}

TEST_F(Multi_Move_List_Qml_Test, controls)
{
    EXPECT_CALL(move_list_slots, added_move(1, -1));
    add_move(&mlq, 1, "01", {});
    EXPECT_CALL(move_list_slots, request_set_current_move_and_branch_start_id(0));
    QMetaObject::invokeMethod(move_list_back_to_start_button, "clicked");
    current_move(&mlq, 0);
    EXPECT_CALL(move_list_slots, added_move(2, 5));
    add_move(&mlq, 2, "02", {5});
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_back());
    QMetaObject::invokeMethod(move_list_back_button, "clicked");
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_forward());
    QMetaObject::invokeMethod(move_list_forward_button, "clicked");
    processEvents();
    EXPECT_EQ(false, QQmlProperty(move_list_root_entry1, "choose_move_list_file_visible").read().toBool());
    QMetaObject::invokeMethod(move_list_import_button, "confirmed");
    processEvents();
    EXPECT_EQ(true, QQmlProperty(move_list_root_entry1, "choose_move_list_file_existing").read().toBool());
    EXPECT_EQ(true, QQmlProperty(move_list_root_entry1, "choose_move_list_file_visible").read().toBool());
    QMetaObject::invokeMethod(move_list_export_button, "clicked");
    processEvents();
    EXPECT_EQ(false, QQmlProperty(move_list_root_entry1, "choose_move_list_file_existing").read().toBool());
    EXPECT_EQ(true, QQmlProperty(move_list_root_entry1, "choose_move_list_file_visible").read().toBool());
}

TEST_F(Multi_Move_List_Qml_Test, lots_of_moves)
{
    add_sequence(1, 100);
    current_move(&mlq, 50);
    EXPECT_EQ(103, move_list_root_buttons->childItems().length()); // controls + trash icon + eye icon + 100 move buttons
    add_sequence(101, 100);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // only 50 move buttons left
    current_move(&mlq, 50);
    add_sequence(201, 100);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length()); // still 50 move buttons left
    current_move(&mlq, 50);
    add_sequence(301, 100);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length());
    current_move(&mlq, 100);
    add_sequence(401, 100); // continue after move 100: ... -> 98 -> 99 -> 100 -> 401 -> 402 -> 403 -> ...
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
    add_sequence(1, 100);
    current_move(&mlq, 50);
    add_sequence(101, 100);
    current_move(&mlq, 150);
    add_sequence(201, 100);
    current_move(&mlq, 250);
    add_sequence(301, 100);

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

TEST_F(Multi_Move_List_Qml_Test, change_color)
{
    add_sequence(1, 30);
    for (int i = 3; i < 28; i += 2) {
        EXPECT_EQ("#111111", QQmlProperty(move_list_root_buttons->childItems()[i], "color").read().toString().toStdString());
        EXPECT_EQ("#222222", QQmlProperty(move_list_root_buttons->childItems()[i + 1], "color").read().toString().toStdString());
    }
    mlq.change_move_color("#111111", "#333333");
    for (int i = 3; i < 28; i += 2) {
        EXPECT_EQ("#333333", QQmlProperty(move_list_root_buttons->childItems()[i], "color").read().toString().toStdString());
        EXPECT_EQ("#222222", QQmlProperty(move_list_root_buttons->childItems()[i + 1], "color").read().toString().toStdString());
    }
    mlq.change_move_color("#222222", "#444444");
    for (int i = 3; i < 28; i += 2) {
        EXPECT_EQ("#333333", QQmlProperty(move_list_root_buttons->childItems()[i], "color").read().toString().toStdString());
        EXPECT_EQ("#444444", QQmlProperty(move_list_root_buttons->childItems()[i + 1], "color").read().toString().toStdString());
    }
}

TEST_F(Multi_Move_List_Qml_Test, change_need_confirm)
{
    add_sequence(1, 30);
    EXPECT_EQ(false, QQmlProperty(move_list_control, "confirm").read().toBool());
    need_confirm(&mlq, true);
    EXPECT_EQ(true, QQmlProperty(move_list_control, "confirm").read().toBool());
    need_confirm(&mlq, false);
    EXPECT_EQ(false, QQmlProperty(move_list_control, "confirm").read().toBool());
}

TEST_F(Multi_Move_List_Qml_Test, import_export)
{
    EXPECT_CALL(move_list_slots, request_move_list_import("/import/path"));
    QMetaObject::invokeMethod(move_list_root_entry1, "chosen_move_list_path", Q_ARG(QVariant, QVariant(QUrl("file:///import/path"))));
    processEvents();
    EXPECT_CALL(move_list_slots, request_move_list_export("/export/path"));
    QQmlProperty(move_list_root_entry1, "choose_move_list_file_existing").write(false);
    QMetaObject::invokeMethod(move_list_root_entry1, "chosen_move_list_path", Q_ARG(QVariant, QVariant(QUrl("file:///export/path"))));
    processEvents();
}

TEST_F(Multi_Move_List_Qml_Test, cut_off_via_button_press)
{
    add_sequence(1, 100);
    EXPECT_EQ(103, move_list_root_buttons->childItems().length());
    current_move(&mlq, 50);
    add_sequence(101, 1000);
    EXPECT_EQ(53, move_list_root_buttons->childItems().length());
    auto entries_from_101 = move_list_root_entry1->childItems().at(2);
    EXPECT_EQ(1002, QQmlProperty(entries_from_101, "buttons").read().value<QQuickItem*>()->childItems().length());
    EXPECT_CALL(move_list_slots, request_delete_branch(101));
    QMetaObject::invokeMethod(QQmlProperty(entries_from_101, "buttons").read().value<QQuickItem*>()->childItems().at(0), "confirmed");
    cut_off(&mlq, 101);
    processEvents();
    EXPECT_EQ(103, move_list_root_buttons->childItems().length());
}

TEST_F(Multi_Move_List_Qml_Test, no_branch_necessary)
{
    add_sequence(1, 100);
    EXPECT_EQ(103, move_list_root_buttons->childItems().length()); // controls + trash icon + eye icon + 100 move buttons
    add_sequence(201, 100);
    EXPECT_EQ(203, move_list_root_buttons->childItems().length()); // Although branch_start_id is 201, there's no need to create a branch, since it would be the only one

    /* content:
     * +---+   +---+   +---+          +---+   +---+   +---+   +---+   +---+          +---+   +---+   +---+
     * |  0+-->+  1+-->+  2+-->...+-->+ 99+-->+100|-->+201+-->+202+-->+203+-->...+-->+298+-->+299|-->+300|
     * +---+   +---+   +---+          +---+   +---+   +---+   +---+   +---+          +---+   +---+   +---+
     */
}
