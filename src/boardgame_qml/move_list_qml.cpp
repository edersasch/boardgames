#include "move_list_qml.h"

#include <QQmlEngine>
#include <QQmlProperty>

namespace boardgame_qml
{

Move_List_Qml::Move_List_Qml(QQmlEngine* engine, QQuickItem* move_list_root_entries)
    : root_entries(move_list_root_entries)
    , move_button(engine, QUrl(QStringLiteral("qrc:/Move_Button.qml")))
    , move_list_entry(engine, QUrl(QStringLiteral("qrc:/Move_List_Entry.qml")))
{
    auto control = QQmlProperty(root_entries, "control").read().value<QQuickItem*>();
    connect(control, SIGNAL(request_move_list_back_to_start()), this, SLOT(request_move_list_back_to_start()));
    connect(control, SIGNAL(request_move_list_forward()), this, SIGNAL(request_move_list_forward()));
    connect(control, SIGNAL(request_move_list_back()), this, SIGNAL(request_move_list_back()));
}

void Move_List_Qml::initial_constellation(const int constellation_id)
{
    move_buttons.clear();
    branches.clear();
    current_move_id = constellation_id;
    start_id = constellation_id;
}

void Move_List_Qml::current_move(const int move_id)
{
    auto button = move_buttons.find(move_id);
    auto curr_button = move_buttons.find(current_move_id);
    if (current_move_id != start_id && curr_button != move_buttons.end()) {
        QQmlProperty(curr_button->second.get(), "highlight").write(false);
    }
    if (button != move_buttons.end()) {
        QQmlProperty(button->second.get(), "highlight").write(true);
        QMetaObject::invokeMethod(button->second->parentItem()->parentItem(), "trigger_move_buttons_visible", Q_ARG(QVariant, true));
    }
    current_move_id = move_id;
}

std::vector<QQuickItem*> Move_List_Qml::add_move(const int move_id, const int branch_id, const std::string& description)
{
    std::vector<QQuickItem*> branch_cols;
    auto buttons = current_move_id == start_id ?
                QQmlProperty(root_entries, "buttons").read().value<QQuickItem*>() :
                move_buttons.at(current_move_id)->parentItem();
    if (move_id == branch_id) {
        auto make_branch = [this, buttons, &branch_cols](int id) -> QQuickItem* {
            auto branch_col = qobject_cast<QQuickItem*>(move_list_entry.create());
            branch_col->setParentItem(buttons->parentItem());
            branch_cols.push_back(branch_col);
            connect(branch_col, SIGNAL(request_delete_branch(int)), this, SIGNAL(request_delete_branch(int)));
            if (current_move_id == start_id) {
                QQmlProperty(branch_col, "leftPadding").write(0);
            }
            branches[id] = std::unique_ptr<QQuickItem>(branch_col);
            return QQmlProperty(branch_col, "buttons").read().value<QQuickItem*>();
        };
        auto reparent_buttons = [this] (QQuickItem* src, QQuickItem* dest) {
            for (auto button : src->childItems()) {
                auto buttonid = QQmlProperty(button, "move_id").read().value<int>();
                if (buttonid > current_move_id) {
                    button->setParentItem(dest);
                }
            }
        };
        for (auto button : buttons->childItems()) {
            if (auto buttonid = QQmlProperty(button, "move_id").read().value<int>(); buttonid > current_move_id) {
                auto buttons_to_move = make_branch(buttonid);
                while (buttons->parentItem()->childItems().length() > 2) {
                    buttons->parentItem()->childItems().at(1)->setParentItem(buttons_to_move->parentItem());
                }
                reparent_buttons(buttons, buttons_to_move);
                break;
            }
        }
        buttons = make_branch(move_id);
    }
    add_move_button(move_id, description);
    move_buttons.at(move_id)->setParentItem(buttons);
    current_move(move_id);
    return branch_cols;
}

void Move_List_Qml::delete_move(const int move_id)
{
    if (move_buttons.find(move_id) != move_buttons.end()) {
        move_buttons.erase(move_id);
    }
}

void Move_List_Qml::cut_off(const int move_id)
{
    auto it = branches.find(move_id);
    if (it != branches.end()) {
        auto reparent_buttons = [] (QQuickItem* src, QQuickItem* dest) {
            for (auto button : src->childItems()) {
                if (!QQmlProperty(button, "move_id").read().isNull()) {
                    button->setParentItem(dest);
                }
            }
        };
        QQuickItem* vParent = it->second->parentItem();
        branches.erase(it);
        if (vParent->childItems().length() == 2) {
            auto vonly_child = vParent->childItems().at(1);
            auto buttons_to_delete = QQmlProperty(vonly_child, "buttons").read().value<QQuickItem*>();
            auto branch_start_id = QQmlProperty(buttons_to_delete->childItems().at(2), "move_id").read().value<int>();
            reparent_buttons(QQmlProperty(vonly_child, "buttons").read().value<QQuickItem*>(), vParent->childItems()[0]);
            while (vonly_child->childItems().length() > 1) {
                vonly_child->childItems()[1]->setParentItem(vParent);
            }
            branches.erase(branches.find(branch_start_id));
        }
    }
}

void Move_List_Qml::set_move_color(const int move_id, const std::string& c)
{
    auto button = move_buttons.find(move_id);
    if (button != move_buttons.end()) {
        QQmlProperty(button->second.get(), "color").write(c.c_str());
    }
}

// private slots

void Move_List_Qml::request_move_list_back_to_start()
{
    emit request_set_current_move_and_branch_start_id(start_id);
}

// private

void Move_List_Qml::add_move_button(const int move_id, const std::string& description)
{
    auto button = qobject_cast<QQuickItem*>(move_button.create());
    QQmlProperty(button, "text").write(description.c_str());
    QQmlProperty(button, "move_id").write(move_id);
    QObject::connect(button, SIGNAL(request_set_move_id(int)), this, SIGNAL(request_set_current_move_and_branch_start_id(int)));
    move_buttons[move_id] = std::unique_ptr<QQuickItem>(button);
}

}
