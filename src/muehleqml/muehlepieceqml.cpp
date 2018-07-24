#include "muehlepieceqml.h"
#include "muehlefieldqml.h"

#include <QUrl>
#include <QQmlProperty>
#include <QQmlEngine>

MuehlePieceQml::MuehlePieceQml(QQmlComponent& pieceComponent, boardgame::PieceNumber id, const char* color)
    : mPieceItem(qobject_cast<QQuickItem*>(pieceComponent.create()))
    , mId(id)
{
    QQmlProperty(mPieceItem.get(), "color").write(color);
    connect(mPieceItem.get(), SIGNAL(removed()), this, SLOT(pieceRemoved()));
    connect(mPieceItem.get(), SIGNAL(selected()), this, SLOT(pieceSelected()));
}

void MuehlePieceQml::setField(MuehleFieldQml& fieldUi)
{
    QQmlProperty(mPieceItem.get(), "nextParent").write(QVariant::fromValue(fieldUi.fieldItem()));
}

void MuehlePieceQml::lock()
{
    QQmlProperty(mPieceItem.get(), "state").write("pin");
}

void MuehlePieceQml::selectable()
{
    QQmlProperty(mPieceItem.get(), "state").write("selectable");
}

void MuehlePieceQml::removable()
{
    QQmlProperty(mPieceItem.get(), "state").write("removable");
}

// private slots

void MuehlePieceQml::pieceRemoved()
{
    emit idPieceRemoved(mId);
}

void MuehlePieceQml::pieceSelected()
{
    emit idPieceSelected(mId);
}
