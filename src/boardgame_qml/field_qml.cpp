#include "boardgame_qml/field_qml.h"

#include <QQmlProperty>
#include <QQuickItem>

namespace boardgame_qml
{

Field_Qml::Field_Qml(QQmlComponent& field_component, boardgame::Field_Number id, QQuickItem* parent_item, QQuickItem* alternative_parent_item)
    : field(qobject_cast<QQuickItem*>(field_component.create()))
    , parent(parent_item)
    , alternative_parent(alternative_parent_item)
    , field_id(id)
{
    field->setParentItem(parent_item);
    connect(field.get(), SIGNAL(occupy()), this, SLOT(occupy()));
}

void Field_Qml::lock()
{
    QQmlProperty(field.get(), "enabled").write("false");
    QQmlProperty(field.get(), "field_opacity").write(0);
}

void Field_Qml::occupiable(const std::string& color)
{
    QQmlProperty(field.get(), "hint_color").write(color.c_str());
    QQmlProperty(field.get(), "enabled").write("true");
    QQmlProperty(field.get(), "field_opacity").write(0);
}

void Field_Qml::highlight()
{
    QQmlProperty(field.get(), "field_color").write("green");
    QQmlProperty(field.get(), "field_opacity").write(0.5);
}

QQuickItem* Field_Qml::get_field()
{
    return field->parentItem();
}

// public slots

void Field_Qml::useMainField()
{
    if (alternative_parent) { // no alternative, no need to change
        for (auto child : alternative_parent->childItems()) {
            child->setParentItem(parent);
        }
    }
}

void Field_Qml::useAlternativeField()
{
    if (alternative_parent) {
        for (auto child : parent->childItems()) {
            child->setParentItem(alternative_parent);
        }
    }
}

// private slots

void Field_Qml::occupy()
{
    emit occupy_id(field_id);
}

}
