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

#include "AppDefaultColors.h"

//----------------------------------------------------------------------------
AppDefaultColors::AppDefaultColors() {

	// Initialize default colors for geometry entities
	// https://doc.qt.io/qt-6/qcolorconstants.html
	_geomEntitiesColors = {
		QColor(200, 220, 230), //!< Isoline
		Qt::green, //!< Free vertex
		Qt::blue, //!< Shared vertex
		Qt::yellow, //!< Free edge
		Qt::cyan, //!< Boundary edge (related to a single face)
		QColor(230, 240, 255), //!< Shared edge (related to several faces)
		Qt::gray, //!< Wireframe face
		QColor(85, 120, 170), //!< Shaded face
		Qt::darkGreen, //!< Seam edge between faces
	};
}

const AppDefaultColors::GeomColorsArray AppDefaultColors::getGeometryEntitiesColorArray() {
	return _geomEntitiesColors;
}
