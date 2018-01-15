#ifndef SRC_MUEHLEQML_MUEHLEFIELDVIEWQML
#define SRC_MUEHLEQML_MUEHLEFIELDVIEWQML

#include "muehlefieldview.h"
#include "muehlepiecemodel.h"

#include <QObject>
#include <QQmlComponent>

#include <vector>
#include <string>
#include <memory>

class QUrl;
class QQmlEngine;
class QQuickItem;

class DestHelper
        : public QObject
{
    Q_OBJECT
public:
    DestHelper(int fieldNumber);
    ~DestHelper() = default;
signals:
    void selected(int);
public slots:
    void helperSelected();
private:
    int mFieldNumber;
};

class MuehleFieldViewQml
        : public QObject
        , public MuehleFieldView
{
    Q_OBJECT
public:
    MuehleFieldViewQml(const QUrl& resource, QQmlEngine* engine, std::vector<QQuickItem*> fields, bool enableDestination = false, std::vector<QQuickItem*> alternativeFields = {});
    virtual ~MuehleFieldViewQml() = default;
    virtual void setPiece(MuehlePieceId p, std::size_t pos) override;
    virtual void normalState(std::size_t pos) override;
    virtual void destinationState(std::size_t pos, const std::string& color) override;
    QQuickItem* item(uint32_t pos);
signals:
    void field(MuehlePieceId, std::size_t);
    void selected(int);
public slots:
    void useMainFields();
    void useAlternativeFields();
private:
    void swapFields(std::vector<QQuickItem*>& src, std::vector<QQuickItem*>& dest);
    QQmlComponent mDestination;
    std::vector<QQuickItem*> mFields;
    std::vector<QQuickItem*> mAlternativeFields;
    std::vector<QQuickItem*> mDestinations {};
    std::vector<QQuickItem*>* mCurrentFields {&mFields};
};

#endif // SRC_MUEHLEQML_MUEHLEFIELDVIEWQML
