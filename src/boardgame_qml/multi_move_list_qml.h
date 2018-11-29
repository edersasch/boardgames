#ifndef SRC_BOARDGAME_QML_MULTI_MOVE_LIST_QML
#define SRC_BOARDGAME_QML_MULTI_MOVE_LIST_QML

#include "move_list_qml.h"

#include <QObject>

#include <memory>
#include <vector>
#include <string>

class QQmlEngine;
class QQuickItem;

namespace boardgame_qml
{

class Multi_Move_List_Qml
        : public QObject
{
    Q_OBJECT
public:
    Multi_Move_List_Qml(QQmlEngine* engine, std::vector<QQuickItem*> move_list_root_entries, const QString& description, const QString& suffix);
    void set_move_color(const int move_id, const std::string& c);

    // from move_list_ui
    friend void initial_constellation(Multi_Move_List_Qml* mui, const int constellation_id);
    friend void current_move(Multi_Move_List_Qml* mui, const int move_id);
    friend void add_move(Multi_Move_List_Qml* mui, const int move_id, const int branch_id, const std::string& description, const std::vector<int>& hint);
    friend void delete_move(Multi_Move_List_Qml* mui, const int move_id);
    friend void cut_off(Multi_Move_List_Qml* mui, const int move_id);

signals:
    void request_set_current_move_and_branch_start_id(int move_id);
    void request_move_list_forward();
    void request_move_list_back();
    void request_move_list_import(std::string url);
    void request_move_list_export(std::string url);
    void request_delete_branch(int move_id);
    void added_move(int move_id, int piece_number);

private slots:
    void chosen_move_list_path(const QVariant& file_urls);

private:
    QQuickItem* file_dialog_root {nullptr};
    std::vector<std::unique_ptr<Move_List_Qml>> move_lists;
};

}

#endif // SRC_BOARDGAME_QML_MULTI_MOVE_LIST_QML
