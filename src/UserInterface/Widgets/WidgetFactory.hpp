/*
 * Copyright (C) 2024 Krystian Fudali
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

#ifndef WIDGETFACTORY_HPP
#define WIDGETFACTORY_HPP

#include "BaseWidget.hpp"
#include <memory>
#include <string>
#include <iostream>

class EntityPickWidget;
class GeometrySignalSender;

/**
 * @brief namespace for functions used for creating widgets
 */
namespace WidgetFactory {

    /**
     * @brief Creates widget that derives from BaseWidget based on the passed widgetName.
     * @param widgetName - name of the widget to create - should match if statement inside implementation
     * @param parent - parent that will be assigned to created widget
     */
    BaseWidget* createWidget(const QString& widgetName, QWidget* parent = nullptr);

}
#endif