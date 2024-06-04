#ifndef TreeWidgetEventHandler_H
#define TreeWidgetEventHandler_H

#include <QObject>
#include <iostream>

class TreeWidgetEventHandler : public QObject {
    Q_OBJECT

public:

signals:
    void entitySelectionConfirmed();
    void selectedEntitiesNamesFetched(const std::vector<std::string>& selectedNames);

private:
};

#endif