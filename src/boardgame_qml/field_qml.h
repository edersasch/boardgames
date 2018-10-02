#ifndef SRC_BOARDGAME_QML_FIELD_QML
#define SRC_BOARDGAME_QML_FIELD_QML

#include "boardgame/pieces_n_fields.h"

#include <QObject>

#include <string>
#include <memory>

class QQmlComponent;
class QQuickItem;

namespace boardgame_qml
{

class Field_Qml
        : public QObject
{
    Q_OBJECT
public:
    Field_Qml(QQmlComponent& field_component, boardgame::Field_Number id, QQuickItem* parent_item, QQuickItem* alternative_parent_item = nullptr);
    void lock();
    void occupiable(const std::string& color);
    void highlight();
    QQuickItem* get_field();
signals:
    void occupy_id(boardgame::Field_Number);
public slots:
    void useMainField();
    void useAlternativeField();
private slots:
    void occupy();
private:
    std::unique_ptr<QQuickItem> field;
    QQuickItem* parent;
    QQuickItem* alternative_parent;
    boardgame::Field_Number field_id;
};

}

#endif // SRC_BOARDGAME_QML_FIELD_QML
