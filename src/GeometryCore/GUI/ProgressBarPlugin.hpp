/*
 * Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool. (https://github.com/PawelekPro/MeshGeneratingTool)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Message_ProgressIndicator.hxx>

#include <QApplication>
#include <QObject>
#include <QProgressDialog>
#include <QWidget>

#include "ProgressBar.h"

/**
 * @brief Provides plugin for abstract interface from program.
 * This includes progress indication and user break mechanisms.
 *
 */
class Progress : public Message_ProgressIndicator {

protected:
	ProgressBar* myProgress;

public:
	DEFINE_STANDARD_RTTIEXT(Progress, Message_ProgressIndicator)

	/**
	 * @brief  Construct class instance.
	 *
	 * @param  {QWidget*} parent  : The parent widget from which it should inherit attributes.
	 */
	Standard_EXPORT Progress(QWidget* parent, const std::string& fileName) {
		myProgress = qobject_cast<ProgressBar*>(parent);
		myProgress->setMinimum(0);
		myProgress->setMaximum(100);
		myProgress->initialize();
		auto message = "Reading file: " + fileName;
		myProgress->setProgressMessage(message.c_str());
		QCoreApplication::processEvents(); // Refresh user interface
	}

	/**
	 * @brief  Class destructor.
	 *
	 */
	Standard_EXPORT ~Progress() {
		myProgress->finish();
	}

	/**
	 * @brief  Reset progress indicator. Overrides virtual base class method.
	 *
	 * @return {Standard_EXPORT}  : Defines platform-specific pre-processor macros necessary for correct compilation.
	 */
	Standard_EXPORT virtual void Reset() Standard_OVERRIDE {
		Message_ProgressIndicator::Reset();
	}

	/**
	 * @brief  Show progress indicator and display progress message. Update presentation of the progress indicator.
	 *
	 * @param  {Message_ProgressScope} theScope : The current scope being advanced.
	 * @param  {Standard_Boolean} force         : Flag isForce is intended for forcing update in case if
	 * it is required at particular step of the algorithm.
	 * @return {Standard_EXPORT}                : Defines platform-specific pre-processor macros necessary for correct compilation.
	 */
	Standard_EXPORT void Show(const Message_ProgressScope& theScope, const Standard_Boolean force = Standard_True) Standard_OVERRIDE {
		// Prepare textual progress info
		std::stringstream aText;
		aText.setf(std::ios::fixed, std::ios::floatfield);
		aText.precision(0);

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
		myProgress->setProgressMessage(aText.str().c_str());
		QApplication::processEvents(); // keep GUI responsive
	}

	/**
	 * @brief  Should return True if user has sent a break signal. This method can be called concurrently,
	 * thus implementation should be thread-safe.
	 *
	 * @return {Standard_EXPORT}  : Defines platform-specific pre-processor macros necessary for correct compilation.
	 */
	Standard_EXPORT virtual Standard_Boolean UserBreak() Standard_OVERRIDE {
		return myProgress->getTerminateIndicator();
	}
};