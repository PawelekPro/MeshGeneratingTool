#ifndef PROGRESSBARPLUGIN_HPP
#define PROGRESSBARPLUGIN_HPP

#include <Message_ProgressIndicator.hxx>
#include <QApplication>
#include <QObject>
#include <QProgressDialog>
#include <QWidget>

#include "ProgressBar.hpp"

class Progress : public Message_ProgressIndicator {
protected:
    ProgressBar* myProgress;

public:
    DEFINE_STANDARD_RTTIEXT(Progress, Message_ProgressIndicator);

    Standard_EXPORT Progress(QWidget* parent, const std::string& fileName);
    Standard_EXPORT ~Progress();

    Standard_EXPORT virtual void Reset() Standard_OVERRIDE;
    Standard_EXPORT void Show(const Message_ProgressScope& theScope,
                              const Standard_Boolean force = Standard_True) Standard_OVERRIDE;
    Standard_EXPORT virtual Standard_Boolean UserBreak() Standard_OVERRIDE;
};

DEFINE_STANDARD_HANDLE(Progress, Message_ProgressIndicator);

#endif  // PROGRESSBARPLUGIN_HPP