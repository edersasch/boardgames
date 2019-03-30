#ifndef TESTS_BOARDGAME_QML_MULTI_MOVE_LIST_QML_TEST
#define TESTS_BOARDGAME_QML_MULTI_MOVE_LIST_QML_TEST

#include "boardgame_qml/multi_move_list_qml.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QQuickView>
#include <QQuickItem>
#include <QObject>

#include <string>
#include <vector>
#include <memory>

void processEvents();

class Multi_Move_List_Qml_Test_Slots
        : public QObject
{
    Q_OBJECT
public:
    virtual ~Multi_Move_List_Qml_Test_Slots() = default;
    MOCK_METHOD1(request_set_current_move_and_branch_start_id, void(const int));
    MOCK_METHOD0(request_move_list_forward, void());
    MOCK_METHOD0(request_move_list_back, void());
    MOCK_METHOD1(request_delete_branch, void(const int));
    MOCK_METHOD2(added_move, void(const int, const int));
    MOCK_METHOD1(request_move_list_import, void(std::string));
    MOCK_METHOD1(request_move_list_export, void(std::string));
};

class Multi_Move_List_Qml_Test
        : public QObject
        , public ::testing::Test
{
    Q_OBJECT
public:
    Multi_Move_List_Qml_Test();
    void add_sequence(int move_id, int number_of_moves);
protected:
    QQuickView v;
    QQmlComponent move_list_component;
    std::unique_ptr<QQuickItem> move_list_item;
    QQuickItem* move_list_root_entry1;
    QQuickItem* move_list_root_entry2;
    QQuickItem* move_list_root_buttons;
    QQuickItem* move_list_control;
    QQuickItem* move_list_back_to_start_button;
    QQuickItem* move_list_back_button;
    QQuickItem* move_list_forward_button;
    QQuickItem* move_list_import_button;
    QQuickItem* move_list_export_button;
    boardgame_qml::Multi_Move_List_Qml mlq;
    ::testing::StrictMock<Multi_Move_List_Qml_Test_Slots> move_list_slots {};
};

#endif // TESTS_BOARDGAME_QML_MULTI_MOVE_LIST_QML_TEST
