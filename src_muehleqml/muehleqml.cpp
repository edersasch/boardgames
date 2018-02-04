#include "muehleqml.h"

#include <QQmlEngine>
#include <QQmlProperty>

#include <vector>

MuehleQml::MuehleQml(QQmlEngine* engine, QQuickItem* parentItem)
    : QObject()
    , MuehleUi()
    , mMuehle(this)
    , mBoard(engine, QUrl(QStringLiteral("qrc:/MuehleBoard.qml")))
    , mBoardItem(qobject_cast<QQuickItem*>(mBoard.create(qmlContext(engine))))
    , mPiece(engine, QUrl(QStringLiteral("qrc:/MuehlePiece.qml")))
    , mDestination(engine, QUrl(QStringLiteral("qrc:/MuehleDestination.qml")))
{
    Q_INIT_RESOURCE(resources);
    auto fillFields = [this, engine](const std::string& repeaterName, const std::string& fgName, std::vector<int> skip = {}, const std::string altrepeaterName = {}) {
        QQuickItem* repeater = QQmlProperty(mBoardItem.get(), repeaterName.c_str()).read().value<QQuickItem*>();
        QQuickItem* altrepeater = nullptr;
        if (!altrepeaterName.empty()) {
            altrepeater = QQmlProperty(mBoardItem.get(), altrepeaterName.c_str()).read().value<QQuickItem*>();
        }
        auto checkSkip = skip.begin();
        int fieldNumber = 0;
        for (int i = 0; i < QQmlProperty(repeater, "count").read().toInt(); i += 1) {
            if (checkSkip == skip.end() || *checkSkip != i) {
                QQuickItem* item = nullptr;
                QQuickItem* altitem = nullptr;
                QMetaObject::invokeMethod(repeater, "itemAt", Q_RETURN_ARG(QQuickItem*, item), Q_ARG(int, i));
                if (altrepeater) {
                    QMetaObject::invokeMethod(altrepeater, "itemAt", Q_RETURN_ARG(QQuickItem*, altitem), Q_ARG(int, i));
                }
                mFields.emplace_back(std::make_unique<MuehleFieldQml>(mDestination, engine, mMuehle.muehleElements()->fieldGroup(fgName)->at(fieldNumber), item, altitem));
                connect(mBoardItem.get(), SIGNAL(horizontal()), mFields.back().get(), SLOT(useMainField()));
                connect(mBoardItem.get(), SIGNAL(vertical()), mFields.back().get(), SLOT(useAlternativeField()));
                fieldNumber += 1;
            } else {
                checkSkip += 1;
            }
        }
    };
    mBoardItem->setParentItem(parentItem);
    for (std::size_t i = 0; i < Muehle::numberOfPieces(); i += 1) {
        mPieces.emplace_back(std::make_unique<MuehlePieceQml>(mPiece, engine, mMuehle.muehleElements()->pieceGroup(Muehle::white())->at(i)));
        mPieces.emplace_back(std::make_unique<MuehlePieceQml>(mPiece, engine, mMuehle.muehleElements()->pieceGroup(Muehle::black())->at(i)));
    }
    fillFields("boardFields", Muehle::board(), {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47});
    fillFields("whiteHFields", Muehle::whitedrawer(), {}, "whiteVFields");
    fillFields("blackHFields", Muehle::blackdrawer(), {}, "blackVFields");
    mMuehle.newGame();
}

void MuehleQml::draw()
{
    ;
}

void MuehleQml::win(const std::string& playerId)
{
    printf("winner %s\n", playerId.c_str());
    mMuehle.newGame();
}
