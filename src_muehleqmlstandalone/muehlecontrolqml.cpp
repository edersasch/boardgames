#include "muehlecontrolqml.h"

#include <QQmlEngine>
#include <QQmlProperty>

MuehleControlQml::MuehleControlQml(QQmlEngine* engine, QQuickItem* parentItem)
    : QObject()
    , mControl(engine, QUrl(QStringLiteral("qrc:/MuehleControl.qml")))
    , mControlItem(qobject_cast<QQuickItem*>(mControl.create()))
    , mMq(engine, QQmlProperty(mControlItem.get(), "gameboard").read().value<QQuickItem*>())
{
    Q_INIT_RESOURCE(pic);
    Q_INIT_RESOURCE(muehlecontrol);
    mControlItem->setParentItem(parentItem);
    connect(mControlItem.get(), SIGNAL(horizontal()), &mMq, SIGNAL(horizontal()));
    connect(mControlItem.get(), SIGNAL(vertical()), &mMq, SIGNAL(vertical()));
    connect(mControlItem.get(), SIGNAL(newGame()), this, SLOT(newGame()));
    connect(mControlItem.get(), SIGNAL(enterSetupMode()), this, SLOT(enterSetupMode()));
    connect(mControlItem.get(), SIGNAL(leaveSetupMode()), this, SLOT(leaveSetupMode()));
}

// private slots

void MuehleControlQml::newGame()
{
    mMq.newGame();
}

void MuehleControlQml::enterSetupMode()
{
    mMq.enterSetupMode();
}

void MuehleControlQml::leaveSetupMode()
{
    mMq.leaveSetupMode();
}
