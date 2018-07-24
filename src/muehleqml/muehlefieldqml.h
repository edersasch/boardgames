#ifndef SRC_MUEHLEQML_MUEHLEFIELDQML
#define SRC_MUEHLEQML_MUEHLEFIELDQML

#include "muehle/muehleui.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <string>
#include <memory>

class QUrl;
class QQmlEngine;

class MuehleFieldQml
        : public QObject
{
    Q_OBJECT
public:
    MuehleFieldQml(QQmlComponent& destinationComponent, boardgame::FieldNumber id, QQuickItem* parentItem, QQuickItem* alternativeParentItem = nullptr);
    void lock();
    void occupiable(const std::string& color);
    void highlight();
    QQuickItem* fieldItem();
signals:
    void idOccupy(boardgame::FieldNumber);
public slots:
    void useMainField();
    void useAlternativeField();
private slots:
    void occupy();
private:
    std::unique_ptr<QQuickItem> mDestinationItem;
    QQuickItem* mParentItem;
    QQuickItem* mAlternativeParentItem;
    boardgame::FieldNumber mId;
};

#endif // SRC_MUEHLEQML_MUEHLEFIELDQML
