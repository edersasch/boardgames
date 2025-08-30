#include "multi_move_list_qml.h"

#include <QDir>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlProperty>
#include <QUrl>

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

void add_move(Multi_Move_List_Qml* mui, const int move_id, const std::string& description, const std::vector<int>& hint)
{
    std::vector<std::vector<QQuickItem*>> all_cols;
    all_cols.reserve(mui->move_lists.size());
    for (auto& ml : mui->move_lists) {
        all_cols.push_back(ml->add_move(move_id, description));
    }
    if (!all_cols.empty()) {
        for (std::size_t i = 0; i < all_cols[0].size(); i += 1) {
            for (std::size_t j = 1; j < all_cols.size(); j += 1) {
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

void need_confirm(Multi_Move_List_Qml* mui, const bool is_needed)
{
    for (auto& ml : mui->move_lists) {
        ml->need_confirm(is_needed);
    }
}

Multi_Move_List_Qml::Multi_Move_List_Qml(QQmlEngine* engine, std::vector<QQuickItem*> move_list_root_entries, const QString& description, const QString& suffix)
{
    move_lists.reserve(move_list_root_entries.size());
    for (auto entry : move_list_root_entries) {
        move_lists.push_back(std::make_unique<Move_List_Qml>(engine, entry));
        connect(move_lists.back().get(), &Move_List_Qml::request_set_current_move_and_branch_start_id, this, &Multi_Move_List_Qml::request_set_current_move_and_branch_start_id);
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_forward, this, &Multi_Move_List_Qml::request_move_list_forward);
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_back, this, &Multi_Move_List_Qml::request_move_list_back);
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_import, this, [this] {
            if (file_dialog_root) {
                QQmlProperty(file_dialog_root, "choose_move_list_file_existing").write(true);
                QQmlProperty(file_dialog_root, "choose_move_list_file_visible").write(true);
            }
        });
        connect(move_lists.back().get(), &Move_List_Qml::request_move_list_export, this, [this] {
            if (file_dialog_root) {
                QQmlProperty(file_dialog_root, "choose_move_list_file_existing").write(false);
                QQmlProperty(file_dialog_root, "choose_move_list_file_visible").write(true);
            }
        });
        connect(move_lists.back().get(), &Move_List_Qml::request_delete_branch, this, &Multi_Move_List_Qml::request_delete_branch);
    }
    if (!move_lists.empty()) {
        file_dialog_root = move_list_root_entries.front();
        QList<QString> name_filters;
        name_filters.push_back(QString(description).append(" (*.").append(suffix).append(")"));
        name_filters.push_back("* (*)");
        QQmlProperty(file_dialog_root, "name_filters").write(QVariant(name_filters));
        QQmlProperty(file_dialog_root, "default_suffix").write(suffix);
        connect(file_dialog_root, SIGNAL(chosen_move_list_path(QVariant)), this, SLOT(chosen_move_list_path(QVariant)));
    }
}

void Multi_Move_List_Qml::set_move_color(const int move_id, const std::string& c)
{
    for (auto& ml : move_lists) {
        ml->set_move_color(move_id, c);
    }
}

void Multi_Move_List_Qml::change_move_color(const QString& old_color, const QString& new_color)
{
    for (auto& ml : move_lists) {
        ml->change_move_color(old_color, new_color);
    }
}

// private slots

void Multi_Move_List_Qml::chosen_move_list_path(const QVariant& file_url)
{
    const auto fileurl = file_url.toUrl();
    const auto localfile = fileurl.toLocalFile();
    const auto filePath = QDir::toNativeSeparators(localfile);
    const auto filestr = filePath.toStdString();
    QQmlProperty(file_dialog_root, "choose_move_list_file_existing").read().toBool() ?
        emit request_move_list_import(filestr) :
        emit request_move_list_export(filestr);
}

}
