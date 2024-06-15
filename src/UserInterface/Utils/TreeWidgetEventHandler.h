#ifndef TreeWidgetEventHandler_H
#define TreeWidgetEventHandler_H

#include <QObject>
#include <iostream>

class TreeWidgetEventHandler : public QObject {
    Q_OBJECT

public:

signals:
    /**
     * Signal that is emitted when the "Select" button in the EntitySelectionWidget is pressed.
     * Signal should be processed outside of TreeStructure
     */
    void entitySelectionConfirmed();
    
    /**
     * Signal that is emitted back to the EntitySelectionWidget containing names of the selected shapes that are to be displayed in its
     * line widget. The signals also contains the tags of the selected shapes that will be saved in the QDomElement.
     * @param selectedNames vector of names that will be displayed in the EntitySelectionWidget.
     * @param selectedTags vector of tags of the vertices that compose selected shapes - will be written in app document.
     */
    void sendNamesToWidgetDisplay(const std::vector<std::string>& selectedNames, std::vector<int> selectedTags);

private:
};

#endif