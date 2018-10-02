#include "multi_move_list_qml.h"

#include <QQmlEngine>
#include <QQuickItem>

namespace boardgame_qml
{

void initial_constellation(Multi_Move_List_Qml* mui, const int constellation_id)
{
    for (auto& ml : mui->move_lists) {
        ml->initial_constellation(constellation_id);
    }
}

void current_move(Multi_Move_List_Qml* mui, const int move_id)
{
    for (auto& ml : mui->move_lists) {
        ml->current_move(move_id);
    }
}

void add_move(Multi_Move_List_Qml* mui, const int move_id, const int branch_id, const std::string& description, const std::vector<int>& hint)
{
    std::vector<std::vector<QQuickItem*>> all_cols;
    for (auto& ml : mui->move_lists) {
        all_cols.push_back(ml->add_move(move_id, branch_id, description));
    }
    if (!all_cols.empty()) {
        for (unsigned i = 0; i < all_cols[0].size(); i += 1) {
            for (unsigned j = 1; j < all_cols.size(); j += 1) {
                QObject::connect(all_cols[j][i], SIGNAL(move_buttons_visible(QVariant)), all_cols[j - 1][i], SLOT(trigger_move_buttons_visible(QVariant)));
                QObject::connect(all_cols[j - 1][i], SIGNAL(move_buttons_visible(QVariant)), all_cols[j][i], SLOT(trigger_move_buttons_visible(QVariant)));
            }
        }
    }
    auto piece_id = -1;
    if (!hint.empty()) {
        piece_id = hint.front();
    }
    emit mui->added_move(move_id, piece_id);
}

void delete_move(Multi_Move_List_Qml* mui, const int move_id)
{
    for (auto& ml : mui->move_lists) {
        ml->delete_move(move_id);
    }
}

void cut_off(Multi_Move_List_Qml* mui, const int move_id)
{
    for (auto& ml : mui->move_lists) {
        ml->cut_off(move_id);
    }
}

Multi_Move_List_Qml::Multi_Move_List_Qml(QQmlEngine* engine, std::vector<QQuickItem*> move_list_root_entries)
{
    for (auto entry : move_list_root_entries) {
        move_lists.push_back(std::make_unique<Move_List_Qml>(engine, entry));
        connect(move_lists.back().get(), &Move_List_Qml::request_set_current_move_and_branch_start_id, this, &Multi_Move_List_Qml::request_set_current_move_and_branch_start_id);
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_forward, this, &Multi_Move_List_Qml::request_move_list_forward);
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_back, this, &Multi_Move_List_Qml::request_move_list_back);
        connect(move_lists.back().get(), &Move_List_Qml::request_delete_branch, this, &Multi_Move_List_Qml::request_delete_branch);
    }
}

void Multi_Move_List_Qml::set_move_color(const int move_id, const std::string& c)
{
    for (auto& ml : move_lists) {
        ml->set_move_color(move_id, c);
    }
}

}
