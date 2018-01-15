#include "muehlefieldviewqml.h"

#include <QUrl>
#include <QQmlProperty>
#include <QQuickItem>
#include <QQmlEngine>

DestHelper::DestHelper(int fieldNumber)
    : QObject()
    , mFieldNumber(fieldNumber)
{
}

void DestHelper::helperSelected()
{
    emit selected(mFieldNumber);
}

MuehleFieldViewQml::MuehleFieldViewQml(const QUrl& resource, QQmlEngine* engine, std::vector<QQuickItem*> fields, bool enableDestination, std::vector<QQuickItem*> alternativeFields)
    : QObject()
    , MuehleFieldView()
    , mDestination(engine, this)
    , mFields(fields)
    , mAlternativeFields(alternativeFields)
{
    if (enableDestination) {
        mDestination.loadUrl(resource);
        for (std::size_t i = 0; i < mFields.size(); i += 1) {
            QQuickItem* d = qobject_cast<QQuickItem*>(mDestination.create(qmlContext(engine)));
            d->setParentItem(mFields.at(i));
            DestHelper* dh = new DestHelper(i);
            connect(d, SIGNAL(selected()), dh, SLOT(helperSelected()));
            connect(dh, &DestHelper::selected, [this] (int fieldNumber) {
                MuehleFieldView::destinationSelected(fieldNumber);
                emit selected(fieldNumber);
            });
            mDestinations.push_back(d);
        }
    }
}

void MuehleFieldViewQml::setPiece(MuehlePieceId p, std::size_t pos)
{
    if (pos < mDestinations.size()) {
        emit field(p, pos);
    }
}

void MuehleFieldViewQml::normalState(size_t pos)
{
    if (pos < mDestinations.size()) {
        QQmlProperty(mDestinations.at(pos), "enabled").write("false");
    }
}

void MuehleFieldViewQml::destinationState(size_t pos, const std::string& color)
{
    if (pos < mDestinations.size()) {
        QQmlProperty(mDestinations.at(pos), "color").write(color.c_str());
        QQmlProperty(mDestinations.at(pos), "enabled").write("true");
    }
}

QQuickItem* MuehleFieldViewQml::item(uint32_t pos)
{
    if (pos < mFields.size()) {
        return mFields.at(pos);
    }
    return nullptr;
}

// public slots

void MuehleFieldViewQml::useMainFields()
{
    swapFields(mAlternativeFields, mFields);
}

void MuehleFieldViewQml::useAlternativeFields()
{
    swapFields(mFields, mAlternativeFields);
}

// private

void MuehleFieldViewQml::swapFields(std::vector<QQuickItem *>& src, std::vector<QQuickItem *>& dest)
{
    if (mCurrentFields != &dest && src.size() == dest.size()) {
        for (std::size_t i = 0; i < src.size(); i += 1) {
            for (auto child : src[i]->childItems()) {
                child->setParentItem(dest[i]);
            }
        }
        mCurrentFields = &dest;
    }
}
