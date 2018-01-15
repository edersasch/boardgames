#include "muehleqml.h"

#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView v;
    MuehleQml mq(v.engine(), v.contentItem());
    v.setWidth(640);
    v.setHeight(480);
    v.show();
    return app.exec();
}
