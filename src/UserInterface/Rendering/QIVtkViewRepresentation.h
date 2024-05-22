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

#ifndef QIVTKVIEWREPRESENTATION_H
#define QIVTKVIEWREPRESENTATION_H

#include "QIVtkSelectionPipeline.h"

#include <vtkLookupTable.h>
#include <vtkSmartPointer.h>

#include <QColor>

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

class QIVtkViewRepresentation {
public:
	QIVtkViewRepresentation();
	~QIVtkViewRepresentation();

	void setRepresentationToShaded(const Handle(QIVtkSelectionPipeline) pipeline) {};
	void setRepresentationToWireframe(const Handle(QIVtkSelectionPipeline) pipeline) {};
	void setRepresentationToPoints(const Handle(QIVtkSelectionPipeline) pipeline) {};
	void setRepresentationToSurfaceWithEdges(const Handle(QIVtkSelectionPipeline) pipeline) {};

	void setEntityColor(QIVtkEntityType entity, QColor color) {};

private:
	vtkSmartPointer<vtkLookupTable> _colorTable;
};

#endif