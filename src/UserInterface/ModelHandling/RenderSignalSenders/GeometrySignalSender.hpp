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

#ifndef GEOMETRYSIGNALSENDER_HPP
#define GEOMETRYSIGNALSENDER_HPP

#include <QObject>

//TODO: maybe replace this with custom enum so that we do not have to link occ everywhere
#include "IVtk_Types.hxx"

/**
 * Class that serves as a collection of signals to handle renderer upon geometry modification.
 * To implement new  render interaction create signal here, implement slot in GeometrySignalHandler and 
 * connect them in mainwindow. 
 */
class GeometrySignalSender : public QObject{
    Q_OBJECT

    public:

    GeometrySignalSender(QObject* aParent);

    /**
     * @brief Uses requestSelectedShapes signal to get ids of currently selected shapes and returns them. 
     */
    const std::vector<int>& getSelectedShapes();
    
    /**
     * @brief Uses requestSelectionType signal to get current selection mode of the renderer. 
     */
    const IVtk_SelectionMode& getSelectionType();

    signals:

    /**
     * @brief Signal to be sent when new gemetry is imported to the model. 
     */
    void geometryImported();

    void requestSelectedShapes();
    void requestSelectionType();

    public slots:

    void receiveSelectedShapes(const std::vector<int>& aSelectedShapes);
    void receiveSelectionType(const IVtk_SelectionMode& aSelectionType);

    private:

    //TODO: I wanted this to be signal/slot only class but i couldnt come up with better
    //      solution to get selected shapes. Storing those as vars here should be reworked.
    
    std::vector<int> _selectedShapes;
    IVtk_SelectionMode _selectionType;

};


#endif