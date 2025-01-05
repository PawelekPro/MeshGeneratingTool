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

#include "QIVtkViewRepresentation.hpp"

//----------------------------------------------------------------------------
QIVtkViewRepresentation::QIVtkViewRepresentation()
	: _colorTable(vtkSmartPointer<QIVtkLookupTable>::New()) {
}

//----------------------------------------------------------------------------
QIVtkViewRepresentation::~QIVtkViewRepresentation() { }

//----------------------------------------------------------------------------
void QIVtkViewRepresentation::setInteractorStyle(
	QVTKInteractorStyle* interactorStyle) {
	_interactorStyle = interactorStyle;
}

//----------------------------------------------------------------------------
void QIVtkViewRepresentation::setRepresentationToShaded() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();

	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator it(pipelinesList);
	for (; it.More(); it.Next()) {
		Handle(QIVtkSelectionPipeline) pipeline = it.Value();
		pipeline->Actor()->GetProperty()->SetRepresentationToSurface();
		pipeline->updatePrimaryPipeline(IVtk_DisplayMode::DM_Shading);
	}
}

//----------------------------------------------------------------------------
void QIVtkViewRepresentation::setRepresentationToSurfaceWithEdges() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();

	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator it(pipelinesList);
	for (; it.More(); it.Next()) {
		Handle(QIVtkSelectionPipeline) pipeline = it.Value();
		pipeline->Actor()->GetProperty()->SetRepresentationToSurface();
		pipeline->updatePrimaryPipeline();
	}
}

//----------------------------------------------------------------------------
void QIVtkViewRepresentation::setRepresentationToWireframe() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();

	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator it(pipelinesList);
	for (; it.More(); it.Next()) {
		Handle(QIVtkSelectionPipeline) pipeline = it.Value();
		pipeline->Actor()->GetProperty()->SetRepresentationToSurface();
		pipeline->Actor()->GetProperty()->SetPointSize(0);
		pipeline->updatePrimaryPipeline(IVtk_DisplayMode::DM_Wireframe);
	}
}

//----------------------------------------------------------------------------
void QIVtkViewRepresentation::setRepresentationToPoints() {
	NCollection_List<Handle(QIVtkSelectionPipeline)> pipelinesList
		= _interactorStyle->getPipelines();

	NCollection_List<Handle(QIVtkSelectionPipeline)>::Iterator it(pipelinesList);
	for (; it.More(); it.Next()) {
		Handle(QIVtkSelectionPipeline) pipeline = it.Value();
		pipeline->updatePrimaryPipeline(IVtk_DisplayMode::DM_Wireframe);
		pipeline->Actor()->GetProperty()->SetRepresentationToPoints();
		pipeline->Actor()->GetProperty()->SetPointSize(5);
	}
}