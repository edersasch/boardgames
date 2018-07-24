#include "muehleqml.h"

#include <QQmlEngine>

#include <iostream>

void draw(MuehleQml* mui)
{
    (void)mui;
}

void win(MuehleQml* mui, const std::string& player)
{
    (void)mui;
    std::cerr << "winner " << player << "\n";
}

void lock_pieces(MuehleQml* mui)
{
    for (auto& p : mui->mPieces) {
        p->lock();
    }
}

void occupiable(MuehleQml* mui, const boardgame::FieldNumber fieldId, const boardgame::PieceNumber pieceId)
{
    mui->mFields.at(static_cast<std::size_t>(fieldId.v))->occupiable(
                pieceId.v < Muehle::firstBlackPiece.v ? mui->board_property("whiteColor").read().toString().toStdString() :
                                                        mui->board_property("blackColor").read().toString().toStdString());
}

MuehleQml::MuehleQml(QQmlEngine* engine, QQuickItem* parentItem)
    : mMuehle(Muehle_Ui_Type(this), Move_List_Ui(&move_lists))
    , mControl(engine, QUrl(QStringLiteral("qrc:/MuehleControl.qml")))
    , mControlItem(qobject_cast<QQuickItem*>(mControl.create()))
    , mBoard(engine, QUrl(QStringLiteral("qrc:/MuehleBoard.qml")))
    , mBoardItem(qobject_cast<QQuickItem*>(mBoard.create()))
    , mPiece(engine, QUrl(QStringLiteral("qrc:/MuehlePiece.qml")))
    , mDestination(engine, QUrl(QStringLiteral("qrc:/MuehleDestination.qml")))
    , move_lists(engine, {board_item("v_move_list"), board_item("h_move_list")})
{
    Q_INIT_RESOURCE(muehle);
    Q_INIT_RESOURCE(boardgame_qml);
    auto fillPieces = [this](const std::string& color, int offset = 0) {
        for (int i = 0; i < Muehle::numberOfPiecesPerPlayer.v; i += 1) {
            mPieces.emplace_back(std::make_unique<MuehlePieceQml>(mPiece, boardgame::PieceNumber{i +  offset}, color.c_str()));
            connect(mPieces.back().get(), &MuehlePieceQml::idPieceRemoved, [this](boardgame::PieceNumber id) {
                mMuehle.piece_removed(id);
            });
            connect(mPieces.back().get(), &MuehlePieceQml::idPieceSelected, [this](boardgame::PieceNumber id) {
                mMuehle.piece_selected(id);
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
                mFields.emplace_back(std::make_unique<MuehleFieldQml>(mDestination, boardgame::FieldNumber{fieldNumber + offset}, item, altitem));
                connect(mControlItem.get(), SIGNAL(horizontal()), mFields.back().get(), SLOT(useMainField()));
                connect(mControlItem.get(), SIGNAL(vertical()), mFields.back().get(), SLOT(useAlternativeField()));
                connect(mFields.back().get(), &MuehleFieldQml::idOccupy, [this](boardgame::FieldNumber id) {
                    mMuehle.occupy(id);
                });
                fieldNumber += 1;
            } else {
                checkSkip += 1;
            }
        }
    };

    mControlItem->setParentItem(parentItem);
    mBoardItem->setParentItem(QQmlProperty(mControlItem.get(), "gameboard").read().value<QQuickItem*>());
    board_property("whiteColor").write("orange");
    board_property("blackColor").write("darkgrey");
    connect(mControlItem.get(), SIGNAL(horizontal()), this, SLOT(horizontal()));
    connect(mControlItem.get(), SIGNAL(vertical()), this, SLOT(vertical()));
    connect(mControlItem.get(), SIGNAL(newGame()), this, SLOT(newGame()));
    connect(mControlItem.get(), SIGNAL(showMoveList()), this, SLOT(showMoveList()));
    connect(mControlItem.get(), SIGNAL(hideMoveList()), this, SLOT(hideMoveList()));
    connect(mControlItem.get(), SIGNAL(enterSetupMode()), this, SLOT(enterSetupMode()));
    connect(mControlItem.get(), SIGNAL(leaveSetupMode()), this, SLOT(leaveSetupMode()));
    fillPieces("orange");
    fillPieces("darkgrey", Muehle::firstBlackPiece.v);
    fillFields("boardFields", Muehle::firstBoardField.v, {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47});
    fillFields("whiteHFields", Muehle::firstWhiteDrawerField.v, {}, "whiteVFields");
    fillFields("blackHFields", Muehle::firstBlackDrawerField.v, {}, "blackVFields");
    fillFields("whiteHPFields", Muehle::firstWhitePrisonField.v, {}, "whiteVPFields");
    fillFields("blackHPFields", Muehle::firstBlackPrisonField.v, {}, "blackVPFields");
    connect(&move_lists, &Multi_Move_List_Qml::request_set_current_move_and_branch_start_id, this, [this](const int move_id) {
        mMuehle.request_set_current_move_and_branch_start_id(move_id);
    });
    connect(&move_lists, &Multi_Move_List_Qml::request_move_list_forward, this, [this]() {
        mMuehle.request_move_list_forward();
    });
    connect(&move_lists, &Multi_Move_List_Qml::request_move_list_back, this, [this]() {
        mMuehle.request_move_list_back();
    });
    connect(&move_lists, &Multi_Move_List_Qml::request_delete_branch, this, [this](const int move_id) {
        mMuehle.request_cut_off(move_id);
    });
    connect(&move_lists, &Multi_Move_List_Qml::added_move, this, [this](const int move_id, const int piece_number) {
        move_lists.set_move_color(move_id, board_property((piece_number >= Muehle::firstBlackPiece.v) ? "blackColor" : "whiteColor").read().toString().toStdString());
    });
    newGame();
}

// private slots

void MuehleQml::horizontal()
{
    board_property("horOrientation").write(true);
}

void MuehleQml::vertical()
{
    board_property("horOrientation").write(false);
}
