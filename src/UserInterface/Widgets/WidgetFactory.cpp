/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#include "WidgetFactory.hpp"
#include "CheckBoxWidget.hpp"
#include "ColorPickerWidget.hpp"
#include "ComboBoxWidget.hpp"
#include "DoubleLineWidget.hpp"
#include "EntityPickWidget.hpp"
#include "IntLineWidget.hpp"
#include "RibbonBarWidget.hpp"
#include "TextLineWidget.hpp"

namespace WidgetFactory {

BaseWidget* createWidget(const QString& widgetName, QWidget* parent) {
	if (widgetName == "ColorPickerWidget") {
		return new ColorPickerWidget(parent);
	} else if (widgetName == "ComboBoxWidget") {
		return new ComboBoxWidget(parent);
	} else if (widgetName == "DoubleLineWidget") {
		return new DoubleLineWidget(parent);
	} else if (widgetName == "EntityPickWidget") {
		return new EntityPickWidget(parent);
	} else if (widgetName == "IntLineWidget") {
		return new IntLineWidget(parent);
	} else if (widgetName == "TextLineWidget") {
		return new TextLineWidget(parent);
	} else if (widgetName == "CheckBoxWidget") {
		return new CheckBoxWidget(parent);
	}

	std::cerr << widgetName.toStdString()
			  << " could not be found in WidgetFactory" << std::endl;
	return nullptr; // Return nullptr if widget type not found
}
}