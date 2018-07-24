#include "muehlefieldqml.h"

#include <QUrl>
#include <QQmlProperty>
#include <QQmlEngine>

MuehleFieldQml::MuehleFieldQml(QQmlComponent& destinationComponent, boardgame::FieldNumber id, QQuickItem* parentItem, QQuickItem* alternativeParentItem)
    : mDestinationItem(qobject_cast<QQuickItem*>(destinationComponent.create()))
    , mParentItem(parentItem)
    , mAlternativeParentItem(alternativeParentItem)
    , mId(id)
{
    mDestinationItem->setParentItem(parentItem);
    connect(mDestinationItem.get(), SIGNAL(occupy()), this, SLOT(occupy()));
}

void MuehleFieldQml::lock()
{
    QQmlProperty(mDestinationItem.get(), "enabled").write("false");
    QQmlProperty(mDestinationItem.get(), "fieldopacity").write(0);
}

void MuehleFieldQml::occupiable(const std::string& color)
{
    QQmlProperty(mDestinationItem.get(), "color").write(color.c_str());
    QQmlProperty(mDestinationItem.get(), "enabled").write("true");
    QQmlProperty(mDestinationItem.get(), "fieldopacity").write(0);
}

void MuehleFieldQml::highlight()
{
    QQmlProperty(mDestinationItem.get(), "fieldcolor").write("green");
    QQmlProperty(mDestinationItem.get(), "fieldopacity").write(0.5);
}

QQuickItem* MuehleFieldQml::fieldItem()
{
    return mDestinationItem->parentItem();
}

// public slots

void MuehleFieldQml::useMainField()
{
    if (mAlternativeParentItem) { // no alternative, no need to change
        for (auto child : mAlternativeParentItem->childItems()) {
            child->setParentItem(mParentItem);
        }
    }
}

void MuehleFieldQml::useAlternativeField()
{
    if (mAlternativeParentItem) {
        for (auto child : mParentItem->childItems()) {
            child->setParentItem(mAlternativeParentItem);
        }
    }
}

// private slots

void MuehleFieldQml::occupy()
{
    emit idOccupy(mId);
}
