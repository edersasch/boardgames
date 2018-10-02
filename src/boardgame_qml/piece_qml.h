#ifndef SRC_BOARDGAME_QML_PIECE_QML
#define SRC_BOARDGAME_QML_PIECE_QML

#include "boardgame/pieces_n_fields.h"

#include <QObject>
#include <QQmlComponent>

#include <memory>

class QQuickItem;

namespace boardgame_qml
{

class Field_Qml;

class Piece_Qml
        : public QObject
{
    Q_OBJECT
public:
    Piece_Qml(QQmlComponent& piece_component, boardgame::Piece_Number id, const char* color);
    void setField(Field_Qml& field);
    void lock();
    void selectable();
    void removable();
signals:
    void removed_id(boardgame::Piece_Number);
    void selected_id(boardgame::Piece_Number);
private slots:
    void removed();
    void selected();
private:
    std::unique_ptr<QQuickItem> piece;
    boardgame::Piece_Number piece_id;
};

}

#endif // SRC_BOARDGAME_QML_PIECE_QML
