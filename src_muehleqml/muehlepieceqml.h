#ifndef SRC_MUEHLEQML_MUEHLEPIECEQML
#define SRC_MUEHLEQML_MUEHLEPIECEQML

#include "muehlepieceui.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <vector>
#include <memory>

class QUrl;
class QQmlEngine;
class MuehleFieldUi;

class MuehlePieceQml
        : public QObject
        , public MuehlePieceUi
{
    Q_OBJECT
public:
    MuehlePieceQml(QQmlComponent& pieceComponent, MuehlePiece* piece);
    virtual ~MuehlePieceQml() = default;
    virtual void fieldChanged(MuehleFieldUi* fieldUi) override;
    virtual void pin() override;
    virtual void selectable() override;
    virtual void removable() override;
private slots:
    void pieceRemoved();
    void pieceSelected();
private:
    std::unique_ptr<QQuickItem> mPieceItem;
};

#endif // SRC_MUEHLEQML_MUEHLEPIECEQML
