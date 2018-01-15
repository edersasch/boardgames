#include "muehlepieceviewqml.h"

#include <QUrl>
#include <QQmlProperty>
#include <QQmlEngine>

MuehlePieceViewQml::MuehlePieceViewQml(const QUrl& resource, QQmlEngine* engine, const QString& pieceColor)
    : QObject()
    , MuehlePieceView(pieceColor.toStdString())
    , mPiece(engine, this)
{
    mPiece.loadUrl(resource);
    for (int i = 0; i < 9; i += 1) {
        QQuickItem* p = qobject_cast<QQuickItem*>(mPiece.create(qmlContext(engine)));
        QQmlProperty(p, "color").write(pieceColor);
        mPieces.push_back(std::unique_ptr<QQuickItem>(p));;
    }
}

void MuehlePieceViewQml::setField(MuehleFieldId f, std::size_t pos)
{
    emit piece(f, pos);
}

void MuehlePieceViewQml::normalState(size_t pos)
{
    QQmlProperty(mPieces.at(pos).get(), "state").write("playerVisible");
}

void MuehlePieceViewQml::canMoveState(size_t pos)
{
    QQmlProperty(mPieces.at(pos).get(), "state").write("playerMoveable");
}

void MuehlePieceViewQml::movingState(size_t pos)
{
    QQmlProperty(mPieces.at(pos).get(), "state").write("playerSelected");
}

QQuickItem* MuehlePieceViewQml::item(uint32_t pos)
{
    if (pos < mPieces.size()) {
        return mPieces.at(pos).get();
    }
    return nullptr;
}

void MuehlePieceViewQml::setParentItem(uint32_t pos, QQuickItem* parent)
{
    if (pos < mPieces.size() && parent != nullptr) {
        mPieces.at(pos)->setParentItem(parent);
    }
}
