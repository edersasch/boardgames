#include "boardgame_qml/piece_qml.h"
#include "boardgame_qml/field_qml.h"

#include <QQuickItem>
#include <QQmlProperty>

namespace boardgame_qml
{

Piece_Qml::Piece_Qml(QQmlComponent& piece_component, boardgame::Piece_Number id, const char* color)
    : piece(qobject_cast<QQuickItem*>(piece_component.create()))
    , piece_id(id)
{
    QQmlProperty(piece.get(), "color").write(color);
    connect(piece.get(), SIGNAL(removed()), this, SLOT(removed()));
    connect(piece.get(), SIGNAL(selected()), this, SLOT(selected()));
    lock();
}

void Piece_Qml::setField(Field_Qml& field)
{
    QQmlProperty(piece.get(), "next_parent").write(QVariant::fromValue(field.get_field()));
}

void Piece_Qml::lock()
{
    QQmlProperty(piece.get(), "state").write("lock");
}

void Piece_Qml::selectable()
{
    QQmlProperty(piece.get(), "state").write("selectable");
}

void Piece_Qml::removable()
{
    QQmlProperty(piece.get(), "state").write("removable");
}

// private slots

void Piece_Qml::removed()
{
    emit removed_id(piece_id);
}

void Piece_Qml::selected()
{
    emit selected_id(piece_id);
}

}
