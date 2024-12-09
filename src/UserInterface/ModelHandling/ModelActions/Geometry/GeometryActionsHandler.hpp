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

#ifndef GEOMETRYACTIONSHANDELR_HPP
#define GEOMETRYACTIONSHANDELR_HPP

#include <memory>
#include <QObject>

#include "BaseActionsHandler.hpp"

class GeometryActionsHandler : public BaseActionsHandler{
    Q_OBJECT

    public:

    GeometryActionsHandler(std::shared_ptr<ModelInterface> aModelInterface, 
                           ProgressBar* aProgressBar, 
                           ModelCommandManager* aModelCommandManager,
                           QObject* aParent);

    void importSTEP();
    void importSTL();

    signals:

    //TODO: make those pass shapes or rags to update only necessary shapes in the render    
    void addShape();
    void removeShape();
    void modifyShape();

    void addShapes();
    void removeShapes();
    void modifyShapes();
};

#endif