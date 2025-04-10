#include "muehle_qml/muehle_qml.h"
#include "boardgame/version.h"

#include <QQmlEngine>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QFile>
#include <QLocale>

#include <thread>

#include <clocale>
#include <cstring>

namespace muehle_qml
{

void draw(Muehle_Qml* ui)
{
    for (std::int32_t i = muehle::first_white_piece.v; i < muehle::number_of_pieces.v; i += 1) {
        QQmlProperty(ui->pieces.at(i).get(), "state").write("draw");
    }
}

void win(Muehle_Qml* ui, const std::string& player)
{
    auto first_piece = muehle::first_white_piece;
    if (player == muehle::black_id) {
        first_piece = muehle::first_black_piece;
    }
    for (std::int32_t i = first_piece.v; i < first_piece.v + muehle::number_of_pieces_per_player.v; i += 1) {
        QQmlProperty(ui->pieces.at(i).get(), "state").write("win");
    }
}

void occupiable(Muehle_Qml* ui, const boardgame::Field_Number fieldId, const boardgame::Piece_Number pieceId)
{
    QQmlProperty(ui->fields.at(static_cast<std::size_t>(fieldId.v)).get(), "state").write("occupiable");
    QQmlProperty(ui->fields.at(static_cast<std::size_t>(fieldId.v)).get(), "hint_color").write(
                pieceId.v < muehle::first_black_piece.v ? ui->board_property("white_color").read().toString() :
                                                          ui->board_property("black_color").read().toString());
}

void active_player(const Muehle_Qml* ui, const std::string& player_id)
{
    QQmlProperty(ui->control.get(), "white_player_active").write(player_id == muehle::white_id);
    QQmlProperty(ui->control.get(), "black_player_active").write(player_id == muehle::black_id);
}

void player_time(const Muehle_Qml* ui, const std::string& player_id, const std::chrono::milliseconds time_in_ms)
{
    static constexpr std::int32_t seconds_per_minute = 60;
    auto s_count = std::chrono::round<std::chrono::seconds>(time_in_ms).count();
    QString seconds = QString::number(s_count % seconds_per_minute);
    if (seconds.size() == 1) {
        seconds = "0" + seconds;
    }
    if (player_id == muehle::white_id) {
        QQmlProperty(ui->control.get(), "white_time").write(QString::number(s_count / seconds_per_minute) + ":" + seconds);
    } else {
        if (player_id == muehle::black_id) {
            QQmlProperty(ui->control.get(), "black_time").write(QString::number(s_count / seconds_per_minute) + ":" + seconds);
        }
    }
}

void engine_forecast(const Muehle_Qml* ui, const std::int32_t score, const std::int32_t depth, const std::vector<std::string>& descriptions)
{
    QString ef1;
    QString ef2;
    QString ef3;
    if (!descriptions.empty()) {
        ef1 = descriptions[0].c_str();
    }
    QQmlProperty(ui->control.get(), "engine_forecast1").write(ef1);
    if (descriptions.size() > 1) {
        ef2 = descriptions[1].c_str();
    }
    QQmlProperty(ui->control.get(), "engine_forecast2").write(ef2);
    if (descriptions.size() > 2) {
        ef3 = descriptions[2].c_str();
    }
    QQmlProperty(ui->control.get(), "engine_forecast3").write(ef3);
    QQmlProperty(ui->control.get(), "engine_score").write(score);
    QQmlProperty(ui->control.get(), "reached_depth").write(depth);
}

Muehle_Qml::Muehle_Qml(QQmlEngine* engine, QQuickItem* parentItem)
    : muehle_state(boardgame::Boardgame_Ui(this), boardgame::Move_List_Ui(&move_lists), boardgame::Main_Loop(this))
    , control_component(engine, QUrl(QStringLiteral("qrc:/src/muehle_qml/Muehle_Control.qml")))
    , control(qobject_cast<QQuickItem*>(control_component.create()))
    , board(QQmlProperty(control.get(), "board").read().value<QQuickItem*>())
    , piece_component(engine, QUrl(QStringLiteral("qrc:/src/boardgame_qml/Piece.qml")))
    , field_component(engine, QUrl(QStringLiteral("qrc:/src/boardgame_qml/Field.qml")))
    , move_lists(engine, {board_item("v_move_list"), board_item("h_move_list")}, "Boardgame Muehle", "bgmu")
{
    static constexpr std::int32_t one_second_in_milliseconds = 1000;
    auto fillPieces = [this](const std::string& color, std::int32_t offset = 0) {
        for (std::int32_t i = 0; i < muehle::number_of_pieces_per_player.v; i += 1) {
            pieces.emplace_back(qobject_cast<QQuickItem*>(piece_component.create()));
            QQmlProperty(pieces.back().get(), "next_color").write(color.c_str());
            QQmlProperty(pieces.back().get(), "piece_id").write(i + offset);
            QQmlProperty(pieces.back().get(), "state").write("lock");
            connect(pieces.back().get(), SIGNAL(removed(int)), this, SLOT(removed(int)));
            connect(pieces.back().get(), SIGNAL(selected(int)), this, SLOT(selected(int)));
        }
    };
    auto fillFields = [this](const std::string& repeaterName, std::int32_t offset, std::vector<int> skip = {}, const std::string& altrepeaterName = {}) {
        auto repeater = board_item(repeaterName);
        QQuickItem* altrepeater = nullptr;
        if (!altrepeaterName.empty()) {
            altrepeater = board_item(altrepeaterName);
        }
        auto checkSkip = skip.begin();
        std::int32_t fieldNumber = 0;
        for (std::int32_t i = 0; i < QQmlProperty(repeater, "count").read().toInt(); i += 1) {
            if (checkSkip == skip.end() || *checkSkip != i) {
                QQuickItem* item = nullptr;
                QQuickItem* altitem = nullptr;
                QMetaObject::invokeMethod(repeater, "itemAt", Q_RETURN_ARG(QQuickItem*, item), Q_ARG(int, i));
                if (altrepeater) {
                    QMetaObject::invokeMethod(altrepeater, "itemAt", Q_RETURN_ARG(QQuickItem*, altitem), Q_ARG(int, i));
                }
                fields.emplace_back(qobject_cast<QQuickItem*>(field_component.create())); //, boardgame::Field_Number{fieldNumber + offset}, item, altitem));
                fields.back().get()->setParentItem(item);
                QQmlProperty(fields.back().get(), "field_id").write(fieldNumber + offset);
                QQmlProperty(fields.back().get(), "normal_parent").write(QVariant::fromValue(item));
                QQmlProperty(fields.back().get(), "alternative_parent").write(QVariant::fromValue(altitem));
                connect(fields.back().get(), SIGNAL(occupy(int)), this, SLOT(occupy(int)));
                fieldNumber += 1;
            } else {
                checkSkip += 1;
            }
        }
    };

    control->setParentItem(parentItem);
    connect(control.get(), SIGNAL(new_game()), this, SLOT(new_game()));
    connect(control.get(), SIGNAL(request_white_engine_active(bool)), this, SLOT(request_white_engine_active(bool)));
    connect(control.get(), SIGNAL(request_black_engine_active(bool)), this, SLOT(request_black_engine_active(bool)));
    connect(control.get(), SIGNAL(request_setup_mode_active(bool)), this, SLOT(request_setup_mode_active(bool)));
    connect(control.get(), SIGNAL(engine_depth(int)), this, SLOT(engine_depth(int)));
    connect(control.get(), SIGNAL(engine_time_in_s(int)), this, SLOT(engine_time_in_s(int)));
    connect(control.get(), SIGNAL(white_color_changed(QString)), this, SLOT(white_color_changed(QString)));
    connect(control.get(), SIGNAL(black_color_changed(QString)), this, SLOT(black_color_changed(QString)));
    connect(control.get(), SIGNAL(show_help()), this, SLOT(show_help()));
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    connect(qApp, &QGuiApplication::lastWindowClosed, this, &Muehle_Qml::end_program);
    fillPieces(board_property("white_color").read().toString().toStdString());
    fillPieces(board_property("black_color").read().toString().toStdString(), muehle::first_black_piece.v);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    fillFields("board_fields", muehle::first_board_field.v, {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47});
    fillFields("white_h_drawer_fields", muehle::first_white_drawer_field.v, {}, "white_v_drawer_fields");
    fillFields("black_h_drawer_fields", muehle::first_black_drawer_field.v, {}, "black_v_drawer_fields");
    fillFields("white_h_prison_fields", muehle::first_white_prison_field.v, {}, "white_v_prison_fields");
    fillFields("black_h_prison_fields", muehle::first_black_prison_field.v, {}, "black_v_prison_fields");
    connect(control.get(), SIGNAL(horizontal()), this, SLOT(use_main_field()));
    connect(control.get(), SIGNAL(vertical()), this, SLOT(use_alternative_field()));
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_set_current_move_and_branch_start_id, this, [this](const std::int32_t move_id) {
        muehle_state.request_set_move_and_branch(move_id);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_forward, this, [this]() {
        muehle_state.request_move_list_forward();
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_back, this, [this]() {
        muehle_state.request_move_list_back();
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_import, this, [this](const std::string& url) {
        muehle_state.request_move_list_import(url);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_move_list_export, this, [this](const std::string& url) {
        muehle_state.request_move_list_export(url);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::request_delete_branch, this, [this](const std::int32_t move_id) {
        muehle_state.request_cut_off(move_id);
    });
    connect(&move_lists, &boardgame_qml::Multi_Move_List_Qml::added_move, this, [this](const std::int32_t move_id, const std::int32_t piece_number) {
        move_lists.set_move_color(move_id, board_property((piece_number >= muehle::first_black_piece.v) ? std::string("black_color") : std::string("white_color")).read().toString().toStdString());
    });
    connect(this, &Muehle_Qml::engine_move, this, [this] {
        muehle_state.engine_move();
    });
    connect(this, &Muehle_Qml::stop_engine, this, [this] {
        muehle_state.stop_engine();
    });
    QQmlProperty(control.get(), "release_info").write(QString(release_info().c_str()));
    connect(&one_second_ticker, &QTimer::timeout, this, [this] {
        muehle_state.tick_1s();
    });
    one_second_ticker.start(one_second_in_milliseconds);
    QTimer::singleShot(0, this, &Muehle_Qml::read_settings);
}

// private slots

void Muehle_Qml::engine_depth(int depth)
{
    muehle_state.set_engine_depth(depth);
    engine_mode = engine_mode_depth;
}

void Muehle_Qml::engine_time_in_s(int time_in_s)
{
    muehle_state.set_engine_time(std::chrono::seconds(time_in_s));
    engine_mode = engine_mode_time;
}

void Muehle_Qml::white_color_changed(const QString& new_white_color)
{
    color_change("white_color", "white_player_active", new_white_color, muehle::first_white_piece, muehle::first_white_prison_field);
}

void Muehle_Qml::black_color_changed(const QString& new_black_color)
{
    color_change("black_color", "black_player_active", new_black_color, muehle::first_black_piece, muehle::first_black_prison_field);
}

void Muehle_Qml::use_main_field()
{
    for (auto& f : fields) {
        QQmlProperty(f.get(), "use_alternative_parent").write(false);
    }
}

void Muehle_Qml::use_alternative_field()
{
    for (auto& f : fields) {
        QQmlProperty(f.get(), "use_alternative_parent").write(true);
    }
}

void Muehle_Qml::show_help() const
{
    QString lang = "en";
    switch (QLocale::system().language()) {
    case QLocale::German:
        lang = "de";
        break;
    default:
        break;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    auto docpath = qApp->applicationDirPath() + "/../share/doc/boardgames/muehle_" + lang + ".html";
    if (!(QFile::permissions(docpath) & QFileDevice::ReadUser && QDesktopServices::openUrl(docpath))) {
        QQmlProperty(control.get(), "no_help_available_visible").write(!QQmlProperty(control.get(), "no_help_available_visible").read().toBool());
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        std::cerr << qPrintable(qApp->platformName()) << '\n' << qPrintable(qApp->applicationDirPath()) << '\n';
        std::cerr << release_info() <<'\n';
    }
}

// private

void Muehle_Qml::read_settings()
{
    static constexpr std::int32_t whiteR = 0xff;
    static constexpr std::int32_t whiteG = 0xfe;
    static constexpr std::int32_t whiteB = 0xcc;
    static constexpr std::int32_t blackR = 0x66;
    static constexpr std::int32_t blackG = 0x33;
    static constexpr std::int32_t blackB = 0x33;
    static constexpr std::int32_t default_engine_time_in_s = 4;
    static constexpr std::int32_t default_engine_depth = 5;
    settings.beginGroup("Ui");
    auto wc = settings.value("white_color", QColor(whiteR, whiteG, whiteB)).toString();
    auto bc = settings.value("black_color", QColor(blackR, blackG, blackB)).toString();
    QQmlProperty(control.get(), "white_color").write(wc);
    QQmlProperty(control.get(), "black_color").write(bc);
    white_color_changed(wc);
    black_color_changed(bc);
    QQmlProperty(control.get(), "move_list_visible").write(settings.value("move_list_visible", false).toBool());
    QQmlProperty(control.get(), "game_info_visible").write(settings.value("game_info_visible", false).toBool());
    settings.endGroup();

    settings.beginGroup("Engine");
    auto et = settings.value("engine_time_in_s", default_engine_time_in_s).toInt();
    auto ed = settings.value("engine_depth", default_engine_depth).toInt();
    QQmlProperty(control.get(), "current_engine_time_in_s").write(et);
    QQmlProperty(control.get(), "current_engine_depth").write(ed);
    engine_time_in_s(et);
    engine_depth(ed);
    engine_mode = settings.value("engine_mode", engine_mode_depth).toString();
    if (engine_mode != engine_mode_depth &&
            engine_mode != engine_mode_time) {
        engine_mode = engine_mode_depth;
    } else {
        QQmlProperty(control.get(), engine_mode).write(true);
    }
    settings.endGroup();

    settings.beginGroup("State");
    muehle_state.request_move_list_import(settings.value("move_list", "").toString().toStdString(), false);
    muehle_state.request_set_move_and_branch(settings.value("move_id").toInt(), settings.value("branch_start_id").toInt());
    muehle_state.request_setup_mode_active(settings.value("setup_mode_active", false).toBool());
    settings.endGroup();
}

void Muehle_Qml::write_settings()
{
    settings.beginGroup("Ui");
    settings.setValue("white_color", QQmlProperty(control.get(), "white_color").read().toString());
    settings.setValue("black_color", QQmlProperty(control.get(), "black_color").read().toString());
    settings.setValue("move_list_visible", QQmlProperty(control.get(), "move_list_visible").read().toBool());
    settings.setValue("game_info_visible", QQmlProperty(control.get(), "game_info_visible").read().toBool());
    settings.endGroup();

    settings.beginGroup("Engine");
    settings.setValue("engine_depth", QQmlProperty(control.get(), "current_engine_depth").read().toInt());
    settings.setValue("engine_time_in_s", QQmlProperty(control.get(), "current_engine_time_in_s").read().toInt());
    settings.setValue("engine_mode", engine_mode);
    settings.endGroup();

    settings.beginGroup("State");
    settings.setValue("move_list", muehle_state.get_move_list_string().c_str());
    settings.setValue("move_id", muehle_state.get_move_id());
    settings.setValue("branch_start_id", muehle_state.get_branch_start_id());
    settings.setValue("setup_mode_active", QQmlProperty(control.get(), "setup_mode_active").read().toBool());
    settings.endGroup();
}

void Muehle_Qml::end_program()
{
    static constexpr std::int32_t hundred_milliseconds = 100;
    static constexpr std::int32_t stop_tries = 20;
    write_settings();
    request_white_engine_active(false);
    request_black_engine_active(false);
    muehle_state.stop_engine();
    for (std::int32_t i = 0; i < stop_tries && muehle_state.is_engine_running(); i += 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(hundred_milliseconds)); // huge transition table needs time to free mem
    }
}

void Muehle_Qml::wait_for_engine_move(std::future<void>&& efu)
{
    static constexpr std::int32_t half_a_second_in_milliseconds = 500;
    if (!efu.valid()) {
        std::cerr << "no engine thread!\n";
        return;
    }
    std::thread([this](std::future<void>&& ef) {
        auto status = ef.wait_for(engine_time > std::chrono::seconds(0) ? engine_time : engine_time_default);
        if (status == std::future_status::ready) {
            emit engine_move();
        } else if (status == std::future_status::timeout) {
            emit stop_engine();
            status = ef.wait_for(std::chrono::milliseconds(half_a_second_in_milliseconds));
            if (status == std::future_status::ready) {
                emit engine_move();
            } else {
                std::cerr << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())).count() << "ms can't stop engine\n";
                ef.wait();
                std::cerr << (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())).count() << "ms engine stopped\n";
                emit engine_move();
            }
        } else {
            std::cerr << "engine thread not running!\n";
        }
    }, std::move(efu)).detach();
}

void Muehle_Qml::color_change(const std::string& color_property_name, const QString& player_active_property_name, const QString& new_color,  boardgame::Piece_Number first_piece, boardgame::Field_Number first_prison_field)
{
    auto old_color = board_property(color_property_name).read().toString();
    for (std::int32_t i = first_piece.v; i < first_piece.v + muehle::number_of_pieces_per_player.v; i += 1) {
        QQmlProperty(pieces.at(i).get(), "next_color").write(new_color);
    }
    if (QQmlProperty(control.get(), player_active_property_name).read().toBool()) {
        for (std::int32_t i = muehle::first_board_field.v; i < muehle::first_board_field.v + muehle::number_of_board_fields.v; i += 1) {
            QQmlProperty(fields.at(i).get(), "hint_color").write(new_color);
        }
        for (std::int32_t i = first_prison_field.v; i < first_prison_field.v + muehle::number_of_prison_fields.v; i += 1) {
            QQmlProperty(fields.at(i).get(), "hint_color").write(new_color);
        }
    }
    move_lists.change_move_color(old_color, new_color);
}

std::string Muehle_Qml::release_info() const
{
    std::string ri = std::to_string(boardgame::major_version) + "." + std::to_string(boardgame::minor_version) + "." + std::to_string(boardgame::patch_version);
    ri += " / " + std::to_string(boardgame::release_year) + "-" + std::to_string(boardgame::release_month) + "-" + std::to_string(boardgame::release_day);
    return ri;
}

}
