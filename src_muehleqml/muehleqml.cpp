#include "muehleqml.h"

#include <QQmlEngine>
#include <QQmlProperty>

#include <vector>

MuehleQml::MuehleQml(QQmlEngine* engine, QQuickItem* parentItem)
    : QObject()
    , MuehleControl()
    , mBoard(engine, QUrl(QStringLiteral("qrc:/MuehleBoard.qml")))
    , mBoardItem(qobject_cast<QQuickItem*>(mBoard.create(qmlContext(engine))))
{
    Q_INIT_RESOURCE(resources);
    auto fillFields = [this](std::string repeaterName, std::vector<int> skip = {}) {
        QQuickItem* repeater = QQmlProperty(mBoardItem.get(), repeaterName.c_str()).read().value<QQuickItem*>();
        std::vector<QQuickItem*> fields {};
        auto checkSkip = skip.begin();
        for (int i = 0; i < QQmlProperty(repeater, "count").read().toInt(); i += 1) {
            if (checkSkip == skip.end() || *checkSkip != i) {
                QQuickItem* f = nullptr;
                QMetaObject::invokeMethod(repeater, "itemAt", Q_RETURN_ARG(QQuickItem*, f), Q_ARG(int, i));
                fields.push_back(f);
            } else {
                checkSkip += 1;
            }
        }
        return fields;
    };
    mBoardItem->setParentItem(parentItem);
    mPieces.insert(std::make_pair(Muehle::white(), std::make_unique<MuehlePieceViewQml>(QUrl("qrc:/MuehlePiece.qml"), engine, "orange")));
    mPieces.insert(std::make_pair(Muehle::black(), std::make_unique<MuehlePieceViewQml>(QUrl("qrc:/MuehlePiece.qml"), engine, "darkgrey")));
    mFields.insert(std::make_pair(Muehle::board(), std::make_unique<MuehleFieldViewQml>(QUrl("qrc:/MuehleDestination.qml"), engine, fillFields("boardFields", {1, 2, 4, 5, 7, 9, 11, 13, 14, 15, 19, 20, 24, 28, 29, 33, 34, 35, 37, 39, 41, 43, 44, 46, 47}), true)));
    mFields.insert(std::make_pair(Muehle::whitedrawer(), std::make_unique<MuehleFieldViewQml>(QUrl("qrc:/MuehleDestination.qml"), engine, fillFields("whiteHFields"), false, fillFields("whiteVFields"))));
    mFields.insert(std::make_pair(Muehle::blackdrawer(), std::make_unique<MuehleFieldViewQml>(QUrl("qrc:/MuehleDestination.qml"), engine, fillFields("blackHFields"), false, fillFields("blackVFields"))));
    connect(mPieces.at(Muehle::white()).get(), &MuehlePieceViewQml::piece, this, &MuehleQml::whitePiece);
    connect(mPieces.at(Muehle::black()).get(), &MuehlePieceViewQml::piece, this, &MuehleQml::blackPiece);
    connect(mBoardItem.get(), SIGNAL(horizontal()), mFields.at(Muehle::whitedrawer()).get(), SLOT(useMainFields()));
    connect(mBoardItem.get(), SIGNAL(vertical()), mFields.at(Muehle::whitedrawer()).get(), SLOT(useAlternativeFields()));
    connect(mBoardItem.get(), SIGNAL(horizontal()), mFields.at(Muehle::blackdrawer()).get(), SLOT(useMainFields()));
    connect(mBoardItem.get(), SIGNAL(vertical()), mFields.at(Muehle::blackdrawer()).get(), SLOT(useAlternativeFields()));
    mMuehle = std::make_unique<Muehle>(this,
                         mPieces.at(Muehle::white()).get(),
                         mPieces.at(Muehle::black()).get(),
                         mFields.at(Muehle::whitedrawer()).get(),
                         mFields.at(Muehle::blackdrawer()).get(),
                         mFields.at(Muehle::whitedrawer()).get(), // reusing drawer as prison
                         mFields.at(Muehle::blackdrawer()).get(), // reusing drawer as prison
                         mFields.at(Muehle::board()).get());
    mMuehle->newGame();
}

void MuehleQml::draw()
{
    ;
}

void MuehleQml::win(std::string playerId)
{
    (void)playerId;
}

// private

void MuehleQml::pieceOnField(MuehlePieceViewQml* p, int ppos, MuehleFieldViewQml* f, int fpos)
{
    p->setParentItem(ppos, f->item(fpos));
}

void MuehleQml::whitePiece(MuehleFieldId f, std::size_t pos)
{
    pieceOnField(mPieces.at(Muehle::white()).get(), pos, mFields.at(f.fieldGroup).get(), f.fieldNumber);
}

void MuehleQml::blackPiece(MuehleFieldId f, std::size_t pos)
{
    pieceOnField(mPieces.at(Muehle::black()).get(), pos, mFields.at(f.fieldGroup).get(), f.fieldNumber);
}
