/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Geometry Generating Tool. (https://github.com/PawelekPro/GeometryGeneratingTool)
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

#include "GeometryRenderHandler.hpp"
#include <TopoDS_Shape.hxx>
#include "ModelDataView.hpp"
#include "QVTKRenderWindow.hpp"

namespace Rendering{

    using PartsMap = std::map<std::string, TopoDS_Shape>;

    GeometryRenderHandler::GeometryRenderHandler(QVTKRenderWindow* aRenderWindow, const ModelDataView& aModelDataView, QObject* aParent) : 
        QObject(aParent),
        _renderWindow(aRenderWindow),
        _modelDataView(aModelDataView){};


    void GeometryRenderHandler::addAllShapesToRenderer(){
        const PartsMap& shapesMap = _modelDataView.getPartsMap();
        _renderWindow->clearRenderer();
        for (const auto shapePair : shapesMap){
            _renderWindow->addShapeToRenderer(shapePair.second);
        }
    }

    void GeometryRenderHandler::showExistingShapes(){
        const PartsMap& shapesMap = _modelDataView.getPartsMap();
        // TODO: clear renderer if we are readding all shapes, alternatively add only required shapes - shapesAdded should take ids as input
        _renderWindow->clearRenderer();
        _renderWindow->addPipelinesToRenderer();
        _renderWindow->RenderScene();
    }

    void GeometryRenderHandler::selectedShapesRequested(){
        const std::vector<std::reference_wrapper<const TopoDS_Shape>>& selectedShapes =  
            _renderWindow->getInteractorStyle()->getSelectedShapes();
        std::vector<int> shapesTags(selectedShapes.size());
        std::transform(selectedShapes.begin(), selectedShapes.end(), shapesTags.begin(),
            [this](const std::reference_wrapper<const TopoDS_Shape>& shape) {
                return _modelDataView.getShapeTag(shape.get());
        });

        emit sendSelectedShapes(shapesTags);
    };

    void GeometryRenderHandler::selectionTypeRequested(){
        IVtk_SelectionMode selectionMode =  
            _renderWindow->getInteractorStyle()->getSelectionMode();
        emit sendSelctionType(selectionMode);
    };
}