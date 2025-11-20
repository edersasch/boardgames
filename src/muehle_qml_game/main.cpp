#include "muehle_qml/muehle_qml.hpp"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <QSettings>

int main(int argc, char *argv[])
{
    static constexpr int initial_window_width = 800;
    static constexpr int initial_window_height = 600;
    const QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Edersasch_Opensource");
    QGuiApplication::setApplicationName("Muehle");

    QSettings settings;
    auto size = settings.value("MainWindow/size", QSize(initial_window_width, initial_window_height)).toSize();
    QQuickView quickView;
    const muehle_qml::Muehle_Qml muehleQml(quickView.engine(), quickView.contentItem());
    quickView.setWidth(size.width());
    quickView.setHeight(size.height());
    quickView.setIcon(QIcon(":/muehle_field_bright.svg"));
    quickView.show();
    auto ret = QGuiApplication::exec();
    size.setWidth(quickView.width());
    size.setHeight(quickView.height());
    settings.setValue("MainWindow/size", size);
    return ret;
}
