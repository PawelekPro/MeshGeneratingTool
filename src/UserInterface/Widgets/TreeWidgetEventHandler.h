#ifndef TreeWidgetEventHandler_H
#define TreeWidgetEventHandler_H

#include <QObject>
#include <iostream>

class TreeWidgetEventHandler : public QObject {
    Q_OBJECT

public:

public slots:
    void entitySelectionConfirmed();  // Corrected the spelling mistake

private slots:

signals:
    void entitiesNamesFetched(std::string name);

private:
};

#endif