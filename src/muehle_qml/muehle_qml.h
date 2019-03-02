#ifndef SRC_MUEHLE_QML_MUEHLE_QML
#define SRC_MUEHLE_QML_MUEHLE_QML

#include "boardgame/boardgame_ui.h"
#include "muehle/muehle_state.h"
#include "boardgame_qml/field_qml.h"
#include "boardgame_qml/piece_qml.h"
#include "boardgame_qml/multi_move_list_qml.h"
#include "boardgame/main_loop.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <memory>
#include <vector>
#include <future>

class QQmlEngine;

namespace muehle_qml
{

class Muehle_Qml
        : public QObject
{
    Q_OBJECT
public:
    Muehle_Qml(QQmlEngine* engine, QQuickItem* parentItem);

    // Boardgame_Ui functions
    friend void draw(Muehle_Qml* mui);
    friend void win(Muehle_Qml* mui, const std::string& player);
    friend void drawer_can_hide(Muehle_Qml* ui, const std::string& player_id, const bool can_hide) { ui->board_property(player_id + "_drawer_can_hide").write(can_hide); }
    friend void prison_can_hide(Muehle_Qml* ui, const std::string& player_id, const bool can_hide) { ui->board_property(player_id + "_prison_can_hide").write(can_hide); }
    friend void lock_piece(Muehle_Qml* ui, const boardgame::Piece_Number pieceId) { ui->pieces.at(static_cast<std::size_t>(pieceId.v))->lock(); }
    friend void selectable(Muehle_Qml* ui, const boardgame::Piece_Number pieceId) { ui->pieces.at(static_cast<std::size_t>(pieceId.v))->selectable(); }
    friend void removable(Muehle_Qml* ui, const boardgame::Piece_Number pieceId) { ui->pieces.at(static_cast<std::size_t>(pieceId.v))->removable(); }
    friend void lock_field(Muehle_Qml* ui, const boardgame::Field_Number fieldId) { ui->fields.at(static_cast<std::size_t>(fieldId.v))->lock(); }
    friend void highlight(Muehle_Qml* ui, const boardgame::Field_Number fieldId) { ui->fields.at(static_cast<std::size_t>(fieldId.v))->highlight(); }
    friend void occupiable(Muehle_Qml* ui, const boardgame::Field_Number fieldId, boardgame::Piece_Number pieceId);
    friend void set_field(Muehle_Qml* ui, const boardgame::Piece_Number pieceId, const boardgame::Field_Number fieldId) { ui->pieces.at(static_cast<std::size_t>(pieceId.v))->setField(*(ui->fields.at(static_cast<std::size_t>(fieldId.v)).get())); }
    friend void setup_mode_active(const Muehle_Qml* ui, const bool is_active) { QQmlProperty(ui->control.get(), "setup_mode_active").write(is_active); }
    friend void engine_active(const Muehle_Qml* ui, const std::string& player_id, const bool is_active) { QQmlProperty(ui->control.get(), (player_id + "_engine_active").c_str()).write(is_active); }
    friend void active_player(const Muehle_Qml* ui, const std::string& player_id);
    friend void need_confirm(Muehle_Qml* ui, const bool is_needed) { QQmlProperty(ui->control.get(), "confirm").write(is_needed); }

    // Main_Loop functions
    friend void engine_future(Muehle_Qml* ml, std::future<bool>&& efu) { ml->wait_for_engine_move(std::move(efu)); }

signals:
    void engine_move(bool);
    void force_engine_move();
private slots:
    void new_game() { muehle_state.new_game(); }
    void show_move_list(bool show) { board_property("show_move_list").write(show); }
    void request_white_engine_active(bool is_active) { muehle_state.request_engine_active(muehle::white_id, is_active); }
    void request_black_engine_active(bool is_active) { muehle_state.request_engine_active(muehle::black_id, is_active); }
    void request_setup_mode_active(bool is_active) { muehle_state.request_setup_mode_active(is_active); }
    void horizontal();
    void vertical();
    void do_engine_move(bool is_valid);
    void do_force_engine_move();
private:
    QQmlProperty board_property(const std::string& property_name) { return QQmlProperty(board.get(), QString(property_name.c_str())); }
    QQuickItem* board_item(const std::string& item_name) { return board_property(item_name).read().value<QQuickItem*>(); }
    void wait_for_engine_move(std::future<bool>&& efu);
    muehle::Muehle_State muehle_state;
    QQmlComponent control_component;
    std::unique_ptr<QQuickItem> control;
    QQmlComponent board_component;
    std::unique_ptr<QQuickItem> board;
    QQmlComponent piece_component;
    std::vector<std::unique_ptr<boardgame_qml::Piece_Qml>> pieces;
    QQmlComponent field_component;
    std::vector<std::unique_ptr<boardgame_qml::Field_Qml>> fields;
    boardgame_qml::Multi_Move_List_Qml move_lists;
};

}

#endif // SRC_MUEHLEQML_MUEHLEQML
