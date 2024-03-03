/* TRASH FILE - MAY BE USEFUL IN THE FUTURE */

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

// https://gitlab.kitware.com/vtk/vtk/-/blob/master/Interaction/Widgets/vtkCameraOrientationWidget.cxx
// https://gitlab.kitware.com/SeunOdutola/vtk/-/blob/master/Interaction/Widgets/vtkCameraOrientationWidget.h
// https://gitlab.kitware.com/vtk/vtk/-/blob/master/Interaction/Widgets/vtkCameraOrientationRepresentation.h
// https://gitlab.kitware.com/vtk/vtk/-/blob/master/Interaction/Widgets/vtkCameraOrientationRepresentation.cxx

#ifndef QVTKNAVIGATIONWIDGET_H
#define QVTKNAVIGATIONWIDGET_H

#include "vtkInteractionWidgetsModule.h"
#include "vtkWidgetRepresentation.h"

namespace Navigation {
const double xyzBaseColor[3][3] = { { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 } };

template <typename EnumT>
constexpr typename std::underlying_type<EnumT>::type to_underlying(const EnumT& e) noexcept {
	return static_cast<typename std::underlying_type<EnumT>::type>(e);
};

class NavigationWidgetRepresentation
	: public vtkWidgetRepresentation {
public:
	static NavigationWidgetRepresentation* New();
	vtkTypeMacro(NavigationWidgetRepresentation, vtkWidgetRepresentation);
	void PrintSelf(ostream& os, vtkIndent indent) override {};

	enum class InteractionStateType : int {
		Outside = 0, // corresponds to vtkCameraOrientationWidget::Inactive
		Hovering, // corresponds to vtkCameraOrientationWidget::Hot
		Rotating // corresponds to vtkCameraOrientationWidget::Active
	};

	/**
	 * Convenient method to get InteractionState as enum.
	 * This method clamps the interaction state to possible values.
	 * Hence, it does not raise any exceptions.
	 */
	InteractionStateType GetInteractionStateAsEnum() noexcept {
		// clamp to 0-2
		this->InteractionState = this->InteractionState < 0 ? 0 : (this->InteractionState > 2 ? 2 : this->InteractionState);
		// convert
		return static_cast<InteractionStateType>(this->InteractionState);
	}

protected:
	NavigationWidgetRepresentation();
	~NavigationWidgetRepresentation() override;
	void RegisterPickers() override {};

private:
	NavigationWidgetRepresentation(const NavigationWidgetRepresentation&) = delete;
	void operator=(const NavigationWidgetRepresentation&) = delete;
};
}

#endif