#include "muehle_qml/muehle_qml.h"

#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Q_INIT_RESOURCE(muehle);
    Q_INIT_RESOURCE(boardgame_qml);
    QQuickView v;
    muehle_qml::Muehle_Qml mq(v.engine(), v.contentItem());
    v.setWidth(640);
    v.setHeight(480);
    v.show();
    return QGuiApplication::exec();
}
