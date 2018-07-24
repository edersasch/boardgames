#ifndef SRC_MUEHLEQML_MUEHLEPIECEQML
#define SRC_MUEHLEQML_MUEHLEPIECEQML

#include "muehle/muehleui.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <memory>

class QUrl;
class QQmlEngine;
class MuehleFieldUi;
class MuehleFieldQml;

class MuehlePieceQml
        : public QObject
{
    Q_OBJECT
public:
    MuehlePieceQml(QQmlComponent& pieceComponent, boardgame::PieceNumber id, const char* color);
    void setField(MuehleFieldQml& fieldUi);
    void lock();
    void selectable();
    void removable();
signals:
    void idPieceRemoved(boardgame::PieceNumber);
    void idPieceSelected(boardgame::PieceNumber);
private slots:
    void pieceRemoved();
    void pieceSelected();
private:
    std::unique_ptr<QQuickItem> mPieceItem;
    boardgame::PieceNumber mId;
};

#endif // SRC_MUEHLEQML_MUEHLEPIECEQML
