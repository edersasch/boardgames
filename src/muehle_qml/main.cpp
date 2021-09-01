#include "muehle_qml/muehle_qml.h"

#include <QGuiApplication>
#include <QQuickView>
#include <QSettings>

int main(int argc, char *argv[])
{
    static constexpr int initial_window_width = 800;
    static constexpr int initial_window_height = 600;
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Edersasch_Opensource");
    QGuiApplication::setApplicationName("Muehle");

    QSettings settings;
    auto size = settings.value("MainWindow/size", QSize(initial_window_width, initial_window_height)).toSize();
    Q_INIT_RESOURCE(muehle);
    Q_INIT_RESOURCE(boardgame_qml);
    QQuickView v;
    muehle_qml::Muehle_Qml mq(v.engine(), v.contentItem());
    v.setWidth(size.width());
    v.setHeight(size.height());
    v.setIcon(QIcon(":/muehle_field_bright.svg"));
    v.show();
    auto ret = QGuiApplication::exec();
    size.setWidth(v.width());
    size.setHeight(v.height());
    settings.setValue("MainWindow/size", size);
    return ret;
}
