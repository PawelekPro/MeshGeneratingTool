#include <Message_ProgressIndicator.hxx>

#include <QApplication>
#include <QProgressDialog>
#include <QWidget>
#include <Qt>
#include <iostream>

// class Progress : public Message_ProgressIndicator {
// public:
// 	DEFINE_STANDARD_RTTIEXT(Progress, Message_ProgressIndicator)

// 	Standard_EXPORT Progress() {};

// 	Standard_EXPORT ~Progress() {};

// 	Standard_EXPORT virtual void Reset() Standard_OVERRIDE {};

// 	Standard_EXPORT void Show(const Message_ProgressScope& theScope, const Standard_Boolean force = Standard_True) Standard_OVERRIDE {};

// 	Standard_EXPORT virtual Standard_Boolean UserBreak() Standard_OVERRIDE { return false; };
// };

class Progress : public Message_ProgressIndicator {
private:
protected:
	QProgressDialog* myProgress;

public:
	DEFINE_STANDARD_RTTIEXT(Progress, Message_ProgressIndicator)

	Standard_EXPORT Progress(QWidget* parent) {
		myProgress = new QProgressDialog(parent);
		myProgress->setMinimum(0);
		myProgress->setMaximum(100);
		myProgress->setWindowModality(Qt::WindowModal);
	}

	Standard_EXPORT ~Progress() {
		if (myProgress)
			delete myProgress;
	}

	Standard_EXPORT virtual void Reset() Standard_OVERRIDE {
		Message_ProgressIndicator::Reset();
	}

	Standard_EXPORT void Show(const Message_ProgressScope& theScope, const Standard_Boolean force = Standard_True) Standard_OVERRIDE {
		// Prepare textual progress info
		std::stringstream aText;
		aText.setf(std::ios::fixed, std::ios::floatfield);
		aText.precision(0);
		aText << "Progress: " << 100. * GetPosition() << "%";
		int progress = 100 * GetPosition();
		myProgress->setValue(progress);
		NCollection_List<const Message_ProgressScope*> aScopes;
		for (const Message_ProgressScope* aPS = &theScope; aPS;
			 aPS = aPS->Parent()) {
			aScopes.Prepend(aPS);
		}

		for (NCollection_List<const Message_ProgressScope*>::Iterator it(aScopes); it.More(); it.Next()) {
			const Message_ProgressScope* aPS = it.Value();
			if (!aPS->Name())
				continue; // skip unnamed scopes
			aText << " " << aPS->Name() << ": ";

			Standard_Real aVal = aPS->Value();
			if (aPS->IsInfinite()) // handle infinite scopes specifically
			{
				if (Precision::IsInfinite(aVal)) {
					aText << "finished";
				} else {
					aText << aVal;
				}
			} else {
				aText << aVal << " / " << aPS->MaxValue();
			}
		}
		std::cout << aText.str().c_str() << "\n";
	}

	Standard_EXPORT virtual Standard_Boolean UserBreak() Standard_OVERRIDE {
		return false;
	}
};
