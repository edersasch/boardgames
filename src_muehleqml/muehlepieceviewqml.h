#ifndef SRC_MUEHLEQML_MUEHLEPIECEVIEWQML
#define SRC_MUEHLEQML_MUEHLEPIECEVIEWQML

#include "muehlepieceview.h"
#include "muehlefieldmodel.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <vector>
#include <memory>

class QUrl;
class QQmlEngine;

class MuehlePieceViewQml
        : public QObject
        , public MuehlePieceView
{
    Q_OBJECT
public:
    MuehlePieceViewQml(const QUrl& resource, QQmlEngine* engine, const QString& pieceColor);
    virtual ~MuehlePieceViewQml() = default;
    virtual void setField(MuehleFieldId f, std::size_t pos) override;
    virtual void normalState(std::size_t pos) override;
    virtual void canMoveState(std::size_t pos) override;
    virtual void movingState(std::size_t pos) override;
    QQuickItem* item(uint32_t pos);
    void setParentItem(uint32_t pos, QQuickItem* parent);
signals:
    void piece(MuehleFieldId, std::size_t);
private:
    QQmlComponent mPiece;
    std::vector<std::unique_ptr<QQuickItem>> mPieces {};
};

#endif // SRC_MUEHLEQML_MUEHLEPIECEVIEW
