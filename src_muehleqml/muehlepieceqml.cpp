#include "muehlepieceqml.h"
#include "muehlepiece.h"
#include "muehlepiecegroup.h"
#include "muehlefield.h"
#include "muehlefieldui.h"
#include "muehlefieldqml.h"

#include <QUrl>
#include <QQmlProperty>
#include <QQmlEngine>

MuehlePieceQml::MuehlePieceQml(QQmlComponent& pieceComponent, MuehlePiece* piece)
    : QObject()
    , MuehlePieceUi(piece)
    , mPieceItem(qobject_cast<QQuickItem*>(pieceComponent.create()))
{
    QQmlProperty(mPieceItem.get(), "color").write(mPiece->pieceGroup()->color().c_str());
    connect(mPieceItem.get(), SIGNAL(removed()), this, SLOT(pieceRemoved()));
    connect(mPieceItem.get(), SIGNAL(selected()), this, SLOT(pieceSelected()));
}

void MuehlePieceQml::fieldChanged(MuehleFieldUi* fieldUi)
{
    MuehleFieldQml* newF = dynamic_cast<MuehleFieldQml*>(fieldUi);
    if (newF) {
        QQmlProperty(mPieceItem.get(), "nextParent").write(QVariant::fromValue(newF->fieldItem()));
    } else { // no fieldUi, so hide the piece
        QQmlProperty(mPieceItem.get(), "state").write("");
    }
}

void MuehlePieceQml::pin()
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
    mPiece->removed();
}

void MuehlePieceQml::pieceSelected()
{
    mPiece->selected();
}
