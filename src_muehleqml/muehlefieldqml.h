#ifndef SRC_MUEHLEQML_MUEHLEFIELDQML
#define SRC_MUEHLEQML_MUEHLEFIELDQML

#include "muehlefieldui.h"
#include "muehlepiecegroup.h"

#include <QObject>
#include <QQmlComponent>
#include <QQuickItem>

#include <vector>
#include <string>
#include <memory>

class QUrl;
class QQmlEngine;

class MuehleFieldQml
        : public QObject
        , public MuehleFieldUi
{
    Q_OBJECT
public:
    MuehleFieldQml(QQmlComponent& destinationComponent, MuehleField* field, QQuickItem* parentItem, QQuickItem* alternativeParentItem = nullptr);
    virtual ~MuehleFieldQml() = default;
    virtual void lock() override;
    virtual void occupiable(const std::string& color) override;
    virtual void highlight() override;
    QQuickItem* fieldItem();
public slots:
    void useMainField();
    void useAlternativeField();
private slots:
    void occupy();
private:
    std::unique_ptr<QQuickItem> mDestinationItem;
    QQuickItem* mParentItem;
    QQuickItem* mAlternativeParentItem;
};

#endif // SRC_MUEHLEQML_MUEHLEFIELDQML
