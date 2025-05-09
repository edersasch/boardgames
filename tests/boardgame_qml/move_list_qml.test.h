#ifndef TESTS_BOARDGAME_QML_MOVE_LIST_QML_TEST
#define TESTS_BOARDGAME_QML_MOVE_LIST_QML_TEST

#include "boardgame_qml/move_list_qml.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QQuickView>
#include <QQuickItem>
#include <QObject>

#include <string>
#include <vector>
#include <memory>

class Move_List_Qml_Test_Slots
        : public QObject
{
    Q_OBJECT
public:
    virtual ~Move_List_Qml_Test_Slots() = default;
    MOCK_METHOD1(request_set_current_move_and_branch_start_id, void(const int));
    MOCK_METHOD0(request_move_list_forward, void());
    MOCK_METHOD0(request_move_list_back, void());
    MOCK_METHOD1(request_delete_branch, void(const int));
};

class Move_List_Qml_Test
        : public QObject
        , public ::testing::Test
{
    Q_OBJECT
public:
    Move_List_Qml_Test();
    std::vector<QQuickItem*> add_sequence(int current_move_id, int move_id, int number_of_moves, int expected_number_of_childItems);
protected:
    QQuickView v;
    QQmlComponent move_list_component;
    std::unique_ptr<QQuickItem> move_list_item;
    QQuickItem* move_list_root_entry {nullptr};
    QQuickItem* move_list_root_buttons {nullptr};
    QQuickItem* move_list_control {nullptr};
    QQuickItem* move_list_back_to_start_button {nullptr};
    QQuickItem* move_list_back_button {nullptr};
    QQuickItem* move_list_forward_button {nullptr};
    std::unique_ptr<boardgame_qml::Move_List_Qml> mlq;
    ::testing::StrictMock<Move_List_Qml_Test_Slots> move_list_slots {};
};

#endif // TESTS_BOARDGAME_QML_MOVE_LIST_QML_TEST
