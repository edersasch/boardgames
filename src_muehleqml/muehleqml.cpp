#include "muehleqml.h"

#include <QQmlEngine>
#include <QQmlProperty>

#include <vector>

MuehleQml::MuehleQml(QQmlEngine* engine, QQuickItem* parentItem)
    : QObject()
    , MuehleUi()
    , mMuehle(this)
    , mBoard(engine, QUrl(QStringLiteral("qrc:/MuehleBoard.qml")))
    , mBoardItem(qobject_cast<QQuickItem*>(mBoard.create()))
    , mPiece(engine, QUrl(QStringLiteral("qrc:/MuehlePiece.qml")))
    , mDestination(engine, QUrl(QStringLiteral("qrc:/MuehleDestination.qml")))
{
    Q_INIT_RESOURCE(muehle);
    MuehleElements* el = mMuehle.muehleElements();
    auto fillFields = [this, engine, el](const std::string& repeaterName, const std::string& fgName, std::vector<int> skip = {}, const std::string altrepeaterName = {}) {
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
                mFields.emplace_back(std::make_unique<MuehleFieldQml>(mDestination, el->fieldGroup(fgName)->at(fieldNumber), item, altitem));
                connect(this, SIGNAL(horizontal()), mFields.back().get(), SLOT(useMainField()));
                connect(this, SIGNAL(vertical()), mFields.back().get(), SLOT(useAlternativeField()));
                fieldNumber += 1;
            } else {
                checkSkip += 1;
            }
        }
    };
    mBoardItem->setParentItem(parentItem);
    connect(this, &MuehleQml::horizontal, this, [this] {
        QQmlProperty(mBoardItem.get(), "horOrientation").write(true);
    });
    connect(this, &MuehleQml::vertical, this, [this] {
        QQmlProperty(mBoardItem.get(), "horOrientation").write(false);
    });
    el->pieceGroup(Muehle::white())->setColor("orange");
    el->pieceGroup(Muehle::black())->setColor("darkgrey");
    QQmlProperty(mBoardItem.get(), "whiteColor").write("orange");
    QQmlProperty(mBoardItem.get(), "blackColor").write("darkgrey");
    for (std::size_t i = 0; i < Muehle::numberOfPieces(); i += 1) {
        mPieces.emplace_back(std::make_unique<MuehlePieceQml>(mPiece, el->pieceGroup(Muehle::white())->at(i)));
        mPieces.emplace_back(std::make_unique<MuehlePieceQml>(mPiece, el->pieceGroup(Muehle::black())->at(i)));
    }
    fillFields("boardFields", Muehle::board(), {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47});
    fillFields("whiteHFields", Muehle::whitedrawer(), {}, "whiteVFields");
    fillFields("blackHFields", Muehle::blackdrawer(), {}, "blackVFields");
    fillFields("whiteHPFields", Muehle::whiteprison(), {}, "whiteVPFields");
    fillFields("blackHPFields", Muehle::blackprison(), {}, "blackVPFields");
    el->fieldGroup(Muehle::whitedrawer())->registerCanHide([this](bool canHide) {
        QQmlProperty(mBoardItem.get(), "whiteDrawerCanHide").write(canHide);
    });
    el->fieldGroup(Muehle::blackdrawer())->registerCanHide([this](bool canHide) {
        QQmlProperty(mBoardItem.get(), "blackDrawerCanHide").write(canHide);
    });
    el->fieldGroup(Muehle::whiteprison())->registerCanHide([this](bool canHide) {
        QQmlProperty(mBoardItem.get(), "whitePrisonCanHide").write(canHide);
    });
    el->fieldGroup(Muehle::blackprison())->registerCanHide([this](bool canHide) {
        QQmlProperty(mBoardItem.get(), "blackPrisonCanHide").write(canHide);
    });
    newGame();
}

void MuehleQml::draw()
{
    ;
}

void MuehleQml::win(const std::string& playerId)
{
    printf("winner %s\n", playerId.c_str());
}

void MuehleQml::newGame()
{
    mMuehle.newGame();
}

void MuehleQml::enterSetupMode()
{
    mMuehle.enterSetupMode();
}

void MuehleQml::leaveSetupMode()
{
    mMuehle.leaveSetupMode();;
}
