#include "muehle_qml/muehle_qml.h"

#include <QGuiApplication>
#include <QQuickView>
#include <QSettings>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Edersasch_Opensource");
    app.setApplicationName("Muehle");

    QSettings settings;
    auto size = settings.value("MainWindow/size", QSize(800, 600)).toSize();
    Q_INIT_RESOURCE(muehle);
    Q_INIT_RESOURCE(boardgame_qml);
    QQuickView v;
    muehle_qml::Muehle_Qml mq(v.engine(), v.contentItem());
    v.setWidth(size.width());
    v.setHeight(size.height());
    v.show();
    auto ret = QGuiApplication::exec();
    size.setWidth(v.width());
    size.setHeight(v.height());
    settings.setValue("MainWindow/size", size);
    return ret;
}
