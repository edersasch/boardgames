#ifndef SRC_MUEHLEQMLSTANDALONE_MUEHLECONTROLQML
#define SRC_MUEHLEQMLSTANDALONE_MUEHLECONTROLQML

#include "muehleqml.h"

#include <QObject>

class MuehleControlQml
        : public QObject
{
    Q_OBJECT
public:
    MuehleControlQml(QQmlEngine* engine, QQuickItem* parentItem = nullptr);
    ~MuehleControlQml() = default;
private slots:
    void newGame();
    void enterSetupMode();
    void leaveSetupMode();
private:
    QQmlComponent mControl;
    std::unique_ptr<QQuickItem> mControlItem;
    MuehleQml mMq;
};

#endif // SRC_MUEHLEQMLSTANDALONE_MUEHLECONTROLQML
