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

#ifndef GEOMETRYRENDERHANDLER_HPP
#define GEOMETRYRENDERHANDLER_HPP

#include <QObject>
#include <IVtk_Types.hxx>

class ModelDataView;

namespace Rendering{

class QVTKRenderWindow;
class RenderSignalHandler;

/**
 * Class for handling all signals sent from UI that require information or action from Renderer
 * To implement a renderer action create a signal in GeometrySignalSender and slot here. Connect
 * Them in mainwindow. This class has access to view the model and should not modify it. 
 * ModelDataView should be used to fetch data from model like for ex. TopoDS_Shapes.
 */
class GeometryRenderHandler : public QObject{
    Q_OBJECT
    friend class RenderSignalHandler;

    public:
    virtual ~GeometryRenderHandler() = default;

    private:
    GeometryRenderHandler(QVTKRenderWindow* aRenderWindow, const ModelDataView& aModelDataView, QObject* aParent);

    public slots:

    // void shapeAdded();
    // void shapeRemoved();
    // void shapeModified();
    
    /**
     * @brief Slot that clears the renderer, fetches all TopoDS_Shapes that are currently in model and
     * passes them to the renderer. Renderer will create selection pipelines and display them.
     */
    void addAllShapesToRenderer();

    /**
    * @brief Slot that clears the renderer. Adds existing selection pipelines to the renderer.
    */
    void showExistingShapes();

    // void shapesRemoved();
    // void shapesModified();

    /**
    * @brief Slot that upon triggered will fetch ids of currently selected TopoDS_Shapes and send them
    * in sendSelectedShapes signal.
    */
    void selectedShapesRequested();

    /**
    * @brief Slot that upon triggered will fetch current SelectionType status from renderer and send it
    * in sendSelectionType signal.
    */
    void selectionTypeRequested();

    signals:

    //TODO: implement class like RenderStateView in ModelHandling so that all signals go only there, to centralized
    // storage. Now signals are send directly to ui which can result in multiple widgets receiving unnecessary signals
    
    /**
    * @brief Signal with current selection type (Vertex, Edge, Face, Volume) of the renderer interactor.
    */
    void sendSelctionType(const IVtk_SelectionMode& aSelectionType);
    
    /**
    * @brief Signal with ids of currently selected shapes
    */
    void sendSelectedShapes(const std::vector<int>& selectedShapes);

    private:
    QVTKRenderWindow* _renderWindow;
    const ModelDataView& _modelDataView;
};

}

#endif