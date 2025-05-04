#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickView>

TEST(Muehle_Qml, init_src)
{
    QQuickView v;
    auto * e = v.engine();
    QQmlComponent muehle_component(e);
    const QStringList muehle_components {"Muehle_Board", "Muehle_Control"};
    for (const auto & component : muehle_components) {
        QUrl url = "qrc:/qt/qml/src/muehle_qml/" + component + ".qml";
        muehle_component.loadUrl(url);
        EXPECT_TRUE(muehle_component.errors().isEmpty());
        if (!muehle_component.errors().isEmpty()) {
            qWarning() << muehle_component.errors();
        }
    }
}

int main(int argc, char **argv)
{
    QGuiApplication app {argc, argv};
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
