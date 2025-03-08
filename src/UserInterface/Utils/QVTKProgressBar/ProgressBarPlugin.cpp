#include "ProgressBarPlugin.hpp"
#include <sstream>
#include <NCollection_List.hxx>
#include <Precision.hxx>

IMPLEMENT_STANDARD_RTTIEXT(Progress, Message_ProgressIndicator);

Progress::Progress(QWidget* parent, const std::string& fileName) {
    myProgress = qobject_cast<ProgressBar*>(parent);
    myProgress->setMinimum(0);
    myProgress->setMaximum(100);
    myProgress->initialize();
    auto message = "Reading file: " + fileName;
    myProgress->setProgressMessage(message.c_str());
    QCoreApplication::processEvents();
}

Progress::~Progress() {
    myProgress->finish();
}

void Progress::Reset() {
    Message_ProgressIndicator::Reset();
}

void Progress::Show(const Message_ProgressScope& theScope, const Standard_Boolean force) {
    std::stringstream aText;
    aText.setf(std::ios::fixed, std::ios::floatfield);
    aText.precision(0);

    int progress = 100 * GetPosition();
    myProgress->setValue(progress);

    NCollection_List<const Message_ProgressScope*> aScopes;
    for (const Message_ProgressScope* aPS = &theScope; aPS; aPS = aPS->Parent()) {
        aScopes.Prepend(aPS);
    }

    for (NCollection_List<const Message_ProgressScope*>::Iterator it(aScopes); it.More(); it.Next()) {
        const Message_ProgressScope* aPS = it.Value();
        if (!aPS->Name()) continue;
        aText << " " << aPS->Name() << ": ";

        Standard_Real aVal = aPS->Value();
        if (aPS->IsInfinite()) {
            if (Precision::IsInfinite(aVal)) {
                aText << "finished";
            } else {
                aText << aVal;
            }
        } else {
            aText << aVal << " / " << aPS->MaxValue();
        }
    }
    myProgress->setProgressMessage(aText.str().c_str());
}

Standard_Boolean Progress::UserBreak() {
    return myProgress->getTerminateIndicator();
}