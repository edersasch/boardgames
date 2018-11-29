#ifndef SRC_BOARDGAME_QML_MOVE_LIST_QML
#define SRC_BOARDGAME_QML_MOVE_LIST_QML

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class QQmlEngine;

namespace boardgame_qml
{

class Move_List_Qml
        : public QObject
{
    Q_OBJECT
public:
    Move_List_Qml(QQmlEngine* engine, QQuickItem* move_list_root_entry);
    void set_move_color(const int move_id, const std::string& c);

    void initial_constellation(const int constellation_id);
    void current_move(const int move_id);
    std::vector<QQuickItem*> add_move(const int move_id, const int branch_id, const std::string& description);
    void delete_move(const int move_id);
    void cut_off(const int move_id);

signals:
    void request_set_current_move_and_branch_start_id(int move_id);
    void request_move_list_forward();
    void request_move_list_back();
    void request_move_list_import();
    void request_move_list_export();
    void request_delete_branch(int move_id);

private slots:
    void request_move_list_back_to_start();

private:
    void add_move_button(const int move_id, const std::string& description);
    QQuickItem* root_entry;
    QQmlComponent move_button;
    QQmlComponent move_list_entry;
    std::string white_color;
    std::string black_color;
    std::unordered_map<int, std::unique_ptr<QQuickItem>> move_buttons; // key is move id
    std::unordered_map<int, std::unique_ptr<QQuickItem>> branches; // key is branch start id
    int current_move_id {-1};
    int start_id {current_move_id};
};

}

#endif // SRC_BOARDGAME_QML_MOVE_LIST_QML
