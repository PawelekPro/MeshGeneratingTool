/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef QIVTKVIEWREPRESENTATION_H
#define QIVTKVIEWREPRESENTATION_H

#include "AppDefaultColors.h"
#include "AppDefaults.h"
#include "QIVtkSelectionPipeline.h"
#include "QIVtkUtils.h"
#include "QVTKInteractorStyle.h"

#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>

#include <QColor>
#include <QList>

/**
 * @class QIVtkViewRepresentation
 * @brief A class to manage VTK view representations.
 *
 * The QIVtkViewRepresentation class encapsulates functionality to set and manage
 * different representations (such as shaded, wireframe, points, and surface with edges)
 * for VTK actors within a QIVtkSelectionPipeline pipeline. It also allows use of a color
 * lookup table for managing colors.
 */
class QIVtkViewRepresentation {
public:
	/**
	 * @brief Constructor for the QIVtkViewRepresentation class.
	 * Initializes a new instance of the QIVtkViewRepresentation class.
	 */
	QIVtkViewRepresentation();

	/**
	 * @brief Destructor for the QIVtkViewRepresentation class.
	 * Cleans up any resources used by the instance.
	 */
	~QIVtkViewRepresentation();

	/**
	 * @brief Sets the interactor style for the view representation.
	 * @param interactorStyle A pointer to the QVTKInteractorStyle instance to be used.
	 */
	void setInteractorStyle(QVTKInteractorStyle* interactorStyle);

	/**
	 * @brief Sets the representation of the actor to shaded (surface).
	 * Changes the current actor's representation to a shaded surface.
	 */
	void setRepresentationToShaded();

	/**
	 * @brief Sets the representation of the actor to wireframe.
	 * Changes the current actor's representation to a wireframe.
	 */
	void setRepresentationToWireframe();

	/**
	 * @brief Sets the representation of the actor to points.
	 * Changes the current actor's representation to points.
	 */
	void setRepresentationToPoints();

	/**
	 * @brief Sets the representation of the actor to surface with edges.
	 * Changes the current actor's representation to display the surface with edges.
	 */
	void setRepresentationToSurfaceWithEdges();

private:
	// Pointer to the interactor style used by this representation.
	vtkSmartPointer<QVTKInteractorStyle> _interactorStyle;

	// Pointer to the color lookup table used by this representation.
	vtkSmartPointer<QIVtkLookupTable> _colorTable;
};

#endif