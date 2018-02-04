#ifndef SRC_MUEHLEQML_MUEHLEQML
#define SRC_MUEHLEQML_MUEHLEQML

#include "muehleui.h"
#include "muehlefieldgroup.h"
#include "muehle.h"
#include "muehlefieldqml.h"
#include "muehlepieceqml.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <memory>
#include <vector>

class Muehle;
class QQmlEngine;

class MuehleQml
        : public QObject
        , public MuehleUi
{
    Q_OBJECT
public:
    MuehleQml(QQmlEngine* engine, QQuickItem* parentItem = nullptr);
    virtual ~MuehleQml() = default;
    void draw() override;
    void win(const std::string& playerId) override;
private:
    Muehle mMuehle;
    QQmlComponent mBoard;
    std::unique_ptr<QQuickItem> mBoardItem;
    QQmlComponent mPiece;
    std::vector<std::unique_ptr<MuehlePieceQml>> mPieces;
    QQmlComponent mDestination;
    std::vector<std::unique_ptr<MuehleFieldQml>> mFields;
};

#endif // SRC_MUEHLEQML_MUEHLEQML
