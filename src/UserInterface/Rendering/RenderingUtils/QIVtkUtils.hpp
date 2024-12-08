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

#ifndef QIVTKUTILS_HPP
#define QIVTKUTILS_HPP

#include "AppDefaultColors.hpp"

#include <QColor>

#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>

typedef enum {
	ET_IsoLine = 0, //!< Isoline
	ET_FreeVertex = 1, //!< Free vertex
	ET_SharedVertex = 2, //!< Shared vertex
	ET_FreeEdge = 3, //!< Free edge
	ET_BoundaryEdge = 4, //!< Boundary edge (related to a single face)
	ET_SharedEdge = 5, //!< Shared edge (related to several faces)
	ET_WireFrameFace = 6, //!< Wireframe face
	ET_ShadedFace = 7, //!< Shaded face
	ET_SeamEdge = 8 //!< Seam edge between faces
} QIVtkEntityType;

/**
 * @class QIVtkLookupTable
 * @brief A custom lookup table class derived from vtkLookupTable.
 *
 * The QIVtkLookupTable class provides a specialized lookup table for color mapping in VTK.
 * It extends the functionality of vtkLookupTable by adding methods for creating instances,
 * retrieving as base class, and setting a custom color table.
 */
class QIVtkLookupTable : public vtkLookupTable {
public:
	/**
	 * @brief Creates a new instance of QIVtkLookupTable.
	 * @return A pointer to a new QIVtkLookupTable instance.
	 */
	static QIVtkLookupTable* New();

	/**
	 * @brief Macro for defining VTK type information.
	 * This macro is used for runtime type information and safe downcasting.
	 */
	vtkTypeMacro(QIVtkLookupTable, vtkLookupTable);

	/**
	 * @brief Retrieves the lookup table as the base vtkLookupTable class.
	 * @return A pointer to the base vtkLookupTable class.
	 */
	vtkLookupTable* GetAsBaseClass();

	/**
	 * @brief Sets the colors table for the lookup table.
	 * @param colorsArray An array of colors to set in the lookup table.
	 */
	void setColorsTable(AppDefaultColors::GeomColorsArray colorsArray);

protected:
	/**
	 * @brief Constructor for the QIVtkLookupTable class.
	 * Initializes a new instance of the QIVtkLookupTable class.
	 */
	QIVtkLookupTable();

	/**
	 * @brief Destructor for the QIVtkLookupTable class.
	 * Cleans up any resources used by the instance.
	 */
	~QIVtkLookupTable();

private:
	/**
	 * @brief Copy constructor (deleted).
	 * Copying of QIVtkLookupTable instances is not allowed.
	 * @param other The other QIVtkLookupTable instance to copy from.
	 */
	QIVtkLookupTable(const QIVtkLookupTable&);

	/**
	 * @brief Assignment operator (deleted).
	 * Assignment of QIVtkLookupTable instances is not allowed.
	 * @param other The other QIVtkLookupTable instance to assign from.
	 * @return A reference to this instance.
	 */
	void operator=(const QIVtkLookupTable&);
};

#endif