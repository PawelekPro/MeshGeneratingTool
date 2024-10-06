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

#ifndef APPDEFAULTCOLORS_H
#define APPDEFAULTCOLORS_H

#include <array>

#include <vtkRenderer.h>

#include <QColor>

class AppDefaultColors {
public:
	typedef std::array<QColor, 9> GeomColorsArray;
	typedef std::array<QColor, 2> RendererColorsArray;
	typedef std::array<QColor, 7> ThemeColorsArray;

	struct ColorSettings {
		GeomColorsArray geomColors;
		RendererColorsArray rendererColors;
		vtkRenderer::GradientModes renBackgroundMode;
		bool isGradientBackgroundOn;
	};

public:
	AppDefaultColors();
	~AppDefaultColors() = default;

public:
	const GeomColorsArray getGeometryEntitiesColorArray(bool defaultColors = false);
	const RendererColorsArray getRendererColorsArray(bool defaultColors = false);
	vtkRenderer::GradientModes getRendererGradientMode(bool defaultVal = false);
	bool isGradientBackgroundEnabled(bool defaultVal = false);

	void setGeometryEntitiesColorArray(GeomColorsArray colorsArray);
	void setRendererColorsArray(RendererColorsArray colorsArray);
	void setRendererGradientMode(vtkRenderer::GradientModes mode);
	void setGradientBackgroundEnabled(bool enabled);

	static void QColorToRgbArray(const QColor& color, double rgb[3]);
	static RendererColorsArray doubleColorsToColorsArray(
		const double* color1, const double* color2);

private:
	ColorSettings _defaultColors;
	ColorSettings _currentColors;
};

#endif