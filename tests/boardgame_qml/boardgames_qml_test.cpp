#include "gtest/gtest.h"

#include <QDirIterator>
#include <QFile>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickView>

TEST(Boardgames_Qml, init_src)
{
    QQuickView v;
    QQmlComponent boardgame_component(v.engine());
    const QStringList boardgame_components {"Move_List_Entry", "Move_List_Root"};
    for (const auto & component : boardgame_components) {
        QUrl url = "qrc:/qt/qml/src/boardgame_qml/" + component + ".qml";
        boardgame_component.loadUrl(url);
        EXPECT_TRUE(boardgame_component.errors().isEmpty());
        if (!boardgame_component.errors().isEmpty()) {
            qWarning() << boardgame_component.errors();
        }
    }
}

TEST(Boardgames_Qml, init_tests)
{
    QQuickView v;
    auto * e = v.engine();
    e->addImportPath("qrc:/");
    QQmlComponent move_list_component(e);
    move_list_component.loadUrl(QUrl("qrc:/qt/qml/tests/boardgame_qml/Move_List_Qml_Test.qml"));
    EXPECT_TRUE(move_list_component.errors().isEmpty());
    if (!move_list_component.errors().isEmpty()) {
        qWarning() << e->importPathList();
        qWarning() << move_list_component.errors();
        for (const auto & dir : {":/qt/qml/src", ":/qt/qml/tests"}) {
            QDirIterator dit(dir, QDirIterator::Subdirectories);
            while (dit.hasNext()) {
                qDebug() << dit.next();
            }
        }
        QFile bqq(":/qt/qml/src/boardgame_qml/qmldir");
        bqq.open(QIODevice::ReadOnly);
        qDebug() << bqq.readAll();
    }
}
