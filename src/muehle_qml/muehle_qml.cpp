#include "muehle_qml/muehle_qml.h"

#include <QQmlEngine>

#include <chrono>
#include <thread>

#include <iostream>

namespace muehle_qml
{

void draw(Muehle_Qml* ui)
{
    (void)ui;
}

void win(Muehle_Qml* ui, const std::string& player)
{
    (void)ui;
    std::cerr << "winner " << player << "\n";
}

void occupiable(Muehle_Qml* ui, const boardgame::Field_Number fieldId, const boardgame::Piece_Number pieceId)
{
    ui->fields.at(static_cast<std::size_t>(fieldId.v))->occupiable(
                pieceId.v < muehle::first_black_piece.v ? ui->board_property("white_color").read().toString().toStdString() :
                                                        ui->board_property("black_color").read().toString().toStdString());
}

void active_player(const Muehle_Qml* ui, const std::string& player_id)
{
    QQmlProperty(ui->control.get(), "white_player_active").write(player_id == muehle::white_id);
    QQmlProperty(ui->control.get(), "black_player_active").write(player_id == muehle::black_id);
}

Muehle_Qml::Muehle_Qml(QQmlEngine* engine, QQuickItem* parentItem)
    : muehle_state(boardgame::Boardgame_Ui(this), boardgame::Move_List_Ui(&move_lists), boardgame::Main_Loop(this))
    , control_component(engine, QUrl(QStringLiteral("qrc:/Muehle_Control.qml")))
    , control(qobject_cast<QQuickItem*>(control_component.create()))
    , board_component(engine, QUrl(QStringLiteral("qrc:/Muehle_Board.qml")))
    , board(qobject_cast<QQuickItem*>(board_component.create()))
    , piece_component(engine, QUrl(QStringLiteral("qrc:/Piece.qml")))
    , field_component(engine, QUrl(QStringLiteral("qrc:/Field.qml")))
    , move_lists(engine, {board_item("v_move_list"), board_item("h_move_list")}, "Boardgame Muehle", "bgmu")
{
    auto fillPieces = [this](const std::string& color, int offset = 0) {
        for (int i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
            pieces.emplace_back(std::make_unique<boardgame_qml::Piece_Qml>(piece_component, boardgame::Piece_Number{i +  offset}, color.c_str()));
            connect(pieces.back().get(), &boardgame_qml::Piece_Qml::removed_id, [this](boardgame::Piece_Number id) {
                muehle_state.piece_removed(id);
            });
            connect(pieces.back().get(), &boardgame_qml::Piece_Qml::selected_id, [this](boardgame::Piece_Number id) {
                muehle_state.request_select_piece(id);
            });
        }
    };
    auto fillFields = [this](const std::string& repeaterName, int offset, std::vector<int> skip = {}, const std::string altrepeaterName = {}) {
        auto* repeater = board_item(repeaterName);
        QQuickItem* altrepeater = nullptr;
        if (!altrepeaterName.empty()) {
            altrepeater = board_item(altrepeaterName);
        }
        auto checkSkip = skip.begin();
        int fieldNumber = 0;
        for (int i = 0; i < QQmlProperty(repeater, "count").read().toInt(); i += 1) {
            if (checkSkip == skip.end() || *checkSkip != i) {
                QQuickItem* item = nullptr;
                QQuickItem* altitem = nullptr;
                QMetaObject::invokeMethod(repeater, "itemAt", Q_RETURN_ARG(QQuickItem*, item), Q_ARG(int, i));
                if (altrepeater) {
                    QMetaObject::invokeMethod(altrepeater, "itemAt", Q_RETURN_ARG(QQuickItem*, altitem), Q_ARG(int, i));
                }
                fields.emplace_back(std::make_unique<boardgame_qml::Field_Qml>(field_component, boardgame::Field_Number{fieldNumber + offset}, item, altitem));
                connect(control.get(), SIGNAL(horizontal()), fields.back().get(), SLOT(useMainField()));
                connect(control.get(), SIGNAL(vertical()), fields.back().get(), SLOT(useAlternativeField()));
                connect(fields.back().get(), &boardgame_qml::Field_Qml::occupy_id, [this](boardgame::Field_Number id) {
                    muehle_state.request_occupy(id);
                });
                fieldNumber += 1;
            } else {
                checkSkip += 1;
            }
        }
    };

    control->setParentItem(parentItem);
    board->setParentItem(QQmlProperty(control.get(), "board").read().value<QQuickItem*>());
    board_property("white_color").write("orange");
    board_property("black_color").write("darkgrey");
    connect(control.get(), SIGNAL(horizontal()), this, SLOT(horizontal()));
    connect(control.get(), SIGNAL(vertical()), this, SLOT(vertical()));
    connect(control.get(), SIGNAL(new_game()), this, SLOT(new_game()));
    connect(control.get(), SIGNAL(show_move_list(bool)), this, SLOT(show_move_list(bool)));
    connect(control.get(), SIGNAL(request_white_engine_active(bool)), this, SLOT(request_white_engine_active(bool)));
    connect(control.get(), SIGNAL(request_black_engine_active(bool)), this, SLOT(request_black_engine_active(bool)));
    connect(control.get(), SIGNAL(request_setup_mode_active(bool)), this, SLOT(request_setup_mode_active(bool)));
    fillPieces(board_property("white_color").read().toString().toStdString());
    fillPieces(board_property("black_color").read().toString().toStdString(), muehle::first_black_piece.v);
    fillFields("board_fields", muehle::first_board_field.v, {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47});
    fillFields("white_h_drawer_fields", muehle::first_white_drawer_field.v, {}, "white_v_drawer_fields");
    fillFields("blackHFields", muehle::first_black_drawer_field.v, {}, "blackVFields");
    fillFields("white_h_prison_fields", muehle::first_white_prison_field.v, {}, "white_v_prison_fields");
    fillFields("blackHPFields", muehle::first_black_prison_field.v, {}, "blackVPFields");
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_set_current_move_and_branch_start_id, this, [this](const int move_id) {
        muehle_state.request_set_current_move_and_branch_start_id(move_id);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_forward, this, [this]() {
        muehle_state.request_move_list_forward();
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_back, this, [this]() {
        muehle_state.request_move_list_back();
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_import, this, [this](std::string url) {
        muehle_state.request_move_list_import(url);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_export, this, [this](std::string url) {
        muehle_state.request_move_list_export(url);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_delete_branch, this, [this](const int move_id) {
        muehle_state.request_cut_off(move_id);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::added_move, this, [this](const int move_id, const int piece_number) {
        move_lists.set_move_color(move_id, board_property((piece_number >= muehle::first_black_piece.v) ? "black_color" : "white_color").read().toString().toStdString());
    });
    connect(this, &Muehle_Qml::engine_move, this, &Muehle_Qml::do_engine_move);
    connect(this, &Muehle_Qml::force_engine_move, this, &Muehle_Qml::do_force_engine_move);
    new_game();
    QQmlProperty(control.get(), "white_engine_color").write(board_property("white_color").read().toString());
    QQmlProperty(control.get(), "black_engine_color").write(board_property("black_color").read().toString());
    QQmlProperty(control.get(), "black_engine_active").write(true);
}

// private slots

void Muehle_Qml::horizontal()
{
    board_property("hor_orientation").write(true);
}

void Muehle_Qml::vertical()
{
    board_property("hor_orientation").write(false);
}

void Muehle_Qml::do_engine_move(bool is_valid)
{
    muehle_state.engine_move(is_valid);
}

void Muehle_Qml::do_force_engine_move()
{
    muehle_state.force_engine_move();
}

// private

void Muehle_Qml::wait_for_engine_move(std::future<bool>&& efu)
{
    if (!efu.valid()) {
        std::cerr << "no engine thread!\n";
        return;
    }
    std::thread([this](std::future<bool>&& ef) {
        auto status = ef.wait_for(std::chrono::milliseconds(3500));
        if (status == std::future_status::ready) {
            emit engine_move(ef.get());
        } else if (status == std::future_status::timeout) {
            emit force_engine_move();
            status = ef.wait_for(std::chrono::milliseconds(500));
            if (status == std::future_status::ready) {
                emit engine_move(ef.get());
            } else {
                std::cerr << "can't stop engine\n";
                ef.wait();
                emit engine_move(ef.get());
            }
        } else {
            std::cerr << "engine thread not running!\n";
        }
    }, std:: move(efu)).detach();
}

}
