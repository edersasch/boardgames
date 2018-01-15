#ifndef SRC_MUEHLEQML_MUEHLEQML
#define SRC_MUEHLEQML_MUEHLEQML

#include "muehlecontrol.h"
#include "muehlefieldmodel.h"
#include "muehle.h"
#include "muehlefieldviewqml.h"
#include "muehlepieceviewqml.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <unordered_map>
#include <memory>

class Muehle;
class QQmlEngine;
class MuehleFieldViewQml;
class MuehlePieceViewQml;

class MuehleQml
        : public QObject
        , public MuehleControl
{
    Q_OBJECT
public:
    MuehleQml(QQmlEngine* engine, QQuickItem* parentItem);
    virtual ~MuehleQml() = default;
    void draw() override;
    void win(std::string playerId) override;
private:
    void pieceOnField(MuehlePieceViewQml* p, int ppos, MuehleFieldViewQml* f, int fpos);
    void whitePiece(MuehleFieldId f, std::size_t pos);
    void blackPiece(MuehleFieldId f, std::size_t pos);
    QQmlComponent mBoard;
    std::unique_ptr<QQuickItem> mBoardItem;
    std::unique_ptr<Muehle> mMuehle {nullptr};
    std::unordered_map<std::string, std::unique_ptr<MuehlePieceViewQml>> mPieces;
    std::unordered_map<std::string, std::unique_ptr<MuehleFieldViewQml>> mFields;
};

#endif // SRC_MUEHLEQML_MUEHLEQML
