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

class GeometrySignalSender : public QObject{
    Q_OBJECT

    public:

    enum class RenderSelectionType{
        Vertex,
        Edge,
        Face,
        Volume
    };

    GeometrySignalSender(QObject* aParent);
    const std::vector<int>& getSelectedShapes();
    const IVtk_SelectionMode& getSelectionType();

    signals:

    // void addShape();
    // void removeShape();
    // void modifyShape();

    void geometryImported();
    // void removeShapes();
    // void modifyShapes();

    void requestSelectedShapes();
    void requestSelectionType();

    public slots:

    void receiveSelectedShapes(const std::vector<int>& aSelectedShapes);
    void receiveSelectionType(const IVtk_SelectionMode& aSelectionType);

    private:

    std::vector<int> _selectedShapes;
    IVtk_SelectionMode _selectionType;

};


#endif