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

#include "AppDefaultColors.hpp"

//----------------------------------------------------------------------------
AppDefaultColors::AppDefaultColors() {

	// Initialize default colors for geometry entities
	// https://doc.qt.io/qt-6/qcolorconstants.html
	_defaultColors.geomColors = {
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

	_defaultColors.rendererColors = {
		QColor(190, 190, 190), // ! Background color 1
		QColor(60, 55, 70) //! Background color 2
	};

	_defaultColors.isGradientBackgroundOn = true;
	_defaultColors.renBackgroundMode
		= vtkRenderer::GradientModes::
			VTK_GRADIENT_RADIAL_VIEWPORT_FARTHEST_CORNER;

	_currentColors = _defaultColors;
}

//----------------------------------------------------------------------------
const AppDefaultColors::GeomColorsArray
AppDefaultColors::getGeometryEntitiesColorArray(bool defaultColors) {
	if (defaultColors) {
		return _defaultColors.geomColors;
	} else {
		return _currentColors.geomColors;
	}
}

//----------------------------------------------------------------------------
void AppDefaultColors::setGeometryEntitiesColorArray(
	AppDefaultColors::GeomColorsArray colorsArray) {
	_currentColors.geomColors = colorsArray;
}

//----------------------------------------------------------------------------
const AppDefaultColors::RendererColorsArray
AppDefaultColors::getRendererColorsArray(bool defaultColors) {
	if (defaultColors) {
		return _defaultColors.rendererColors;
	} else {
		return _currentColors.rendererColors;
	}
}

//----------------------------------------------------------------------------
void AppDefaultColors::setRendererColorsArray(
	AppDefaultColors::RendererColorsArray colorsArray) {
	_currentColors.rendererColors = colorsArray;
}

//----------------------------------------------------------------------------
vtkRenderer::GradientModes AppDefaultColors::getRendererGradientMode(bool defaultVal) {
	if (defaultVal) {
		return _defaultColors.renBackgroundMode;
	} else {
		return _currentColors.renBackgroundMode;
	}
}

//----------------------------------------------------------------------------
void AppDefaultColors::setRendererGradientMode(
	vtkRenderer::GradientModes mode) {
	_currentColors.renBackgroundMode = mode;
}

//----------------------------------------------------------------------------
bool AppDefaultColors::isGradientBackgroundEnabled(bool defaultVal) {
	if (defaultVal) {
		return _defaultColors.isGradientBackgroundOn;
	} else {
		return _currentColors.isGradientBackgroundOn;
	}
}

//----------------------------------------------------------------------------
void AppDefaultColors::setGradientBackgroundEnabled(bool enabled) {
	_currentColors.isGradientBackgroundOn = enabled;
}

//----------------------------------------------------------------------------
void AppDefaultColors::QColorToRgbArray(const QColor& color, double rgb[3]) {
	float r, g, b;
	color.getRgbF(&r, &g, &b);
	rgb[0] = static_cast<double>(r);
	rgb[1] = static_cast<double>(g);
	rgb[2] = static_cast<double>(b);
}

//----------------------------------------------------------------------------
AppDefaultColors::RendererColorsArray AppDefaultColors::doubleColorsToColorsArray(
	const double* color1, const double* color2) {
	RendererColorsArray array;

	array[0] = QColor::fromRgbF(color1[0], color1[1], color1[2]);
	array[1] = QColor::fromRgbF(color2[0], color2[1], color2[2]);
	return array;
}