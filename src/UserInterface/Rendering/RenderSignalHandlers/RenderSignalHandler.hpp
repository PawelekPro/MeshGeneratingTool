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

#ifndef RENDERSIGNALHANDLER_HPP
#define RENDERSIGNALHANDLER_HPP

#include <QObject>
#include "QVTKRenderWindow.hpp"
#include "MeshRenderHandler.hpp"
#include "GeometryRenderHandler.hpp"
#include "ModelDataView.hpp"

namespace Rendering{

class RenderSignalHandler : public QObject{
    Q_OBJECT

    public:
    RenderSignalHandler(QVTKRenderWindow* aRenderWindow, const ModelDataView& aModelDataView, QObject* aParent);
    ~RenderSignalHandler() = default;


    // MeshRenderHandler* mesh(){return _meshRenderHandler;};
    GeometryRenderHandler* geometry(){return _geometryRenderHandler;};

    private:

    QVTKRenderWindow* _renderWindow;
    const ModelDataView& _modelDataView;


    MeshRenderHandler* _meshRenderHandler;
    GeometryRenderHandler* _geometryRenderHandler;

    public slots:

    private slots:

};

}

#endif