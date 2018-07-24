#ifndef SRC_MUEHLEQML_MUEHLEQML
#define SRC_MUEHLEQML_MUEHLEQML

#include "muehle/muehleui.h"
#include "muehle/muehle.h"
#include "muehleqml/muehlefieldqml.h"
#include "muehleqml/muehlepieceqml.h"
#include "boardgame_qml/multi_move_list_qml.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlProperty>

#include <memory>
#include <vector>

class QQmlEngine;
class MuehleQml;

class MuehleQml
        : public QObject
{
    Q_OBJECT
public:
    MuehleQml(QQmlEngine* engine, QQuickItem* parentItem);

    // MuehleUi functions
    friend void draw(MuehleQml* mui);
    friend void win(MuehleQml* mui, const std::string& playerId);
    friend void white_drawer_can_hide(MuehleQml* mui, const bool canHide) { mui->board_property("whiteDrawerCanHide").write(canHide); }
    friend void black_drawer_can_hide(MuehleQml* mui, const bool canHide) { mui->board_property("blackDrawerCanHide").write(canHide); }
    friend void white_prison_can_hide(MuehleQml* mui, const bool canHide) { mui->board_property("whitePrisonCanHide").write(canHide); }
    friend void black_prison_can_hide(MuehleQml* mui, const bool canHide) { mui->board_property("blackPrisonCanHide").write(canHide); }
    friend void lock_pieces(MuehleQml* mui);
    friend void selectable(MuehleQml* mui, const boardgame::PieceNumber pieceId) { mui->mPieces.at(static_cast<std::size_t>(pieceId.v))->selectable(); }
    friend void removable(MuehleQml* mui, const boardgame::PieceNumber pieceId) { mui->mPieces.at(static_cast<std::size_t>(pieceId.v))->removable(); }
    friend void lock_field(MuehleQml* mui, const boardgame::FieldNumber fieldId) { mui->mFields.at(static_cast<std::size_t>(fieldId.v))->lock(); }
    friend void highlight(MuehleQml* mui, const boardgame::FieldNumber fieldId) { mui->mFields.at(static_cast<std::size_t>(fieldId.v))->highlight(); }
    friend void occupiable(MuehleQml* mui, const boardgame::FieldNumber fieldId, boardgame::PieceNumber pieceId);
    friend void set_field(MuehleQml* mui, const boardgame::PieceNumber pieceId, const boardgame::FieldNumber fieldId) { mui->mPieces.at(static_cast<std::size_t>(pieceId.v))->setField(*(mui->mFields.at(static_cast<std::size_t>(fieldId.v)).get())); }
private slots:
    void newGame() { mMuehle.new_game(); }
    void showMoveList() { board_property("showMoveList").write(true); }
    void hideMoveList() { board_property("showMoveList").write(false); }
    void enterSetupMode() { mMuehle.enter_setup_mode(); }
    void leaveSetupMode() { mMuehle.leave_setup_mode(); }
    void horizontal();
    void vertical();
private:
    QQmlProperty board_property(const std::string& property_name) { return QQmlProperty(mBoardItem.get(), QString(property_name.c_str())); }
    QQuickItem* board_item(const std::string& item_name) { return board_property(item_name).read().value<QQuickItem*>(); }
    Muehle mMuehle;
    QQmlComponent mControl;
    std::unique_ptr<QQuickItem> mControlItem;
    QQmlComponent mBoard;
    std::unique_ptr<QQuickItem> mBoardItem;
    QQmlComponent mPiece;
    std::vector<std::unique_ptr<MuehlePieceQml>> mPieces;
    QQmlComponent mDestination;
    std::vector<std::unique_ptr<MuehleFieldQml>> mFields;
    Multi_Move_List_Qml move_lists;
};

#endif // SRC_MUEHLEQML_MUEHLEQML
