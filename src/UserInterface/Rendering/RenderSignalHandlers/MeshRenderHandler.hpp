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

#ifndef MESHRENDERHANDLER_HPP
#define MESHRENDERHANDLER_HPP

#include <QObject>
#include <IVtk_Types.hxx>

class ModelDataView;

namespace Rendering{

class QVTKRenderWindow;
class RenderSignalHandler;

class MeshRenderHandler : public QObject{
    Q_OBJECT
    friend class RenderSignalHandler;

    public:
    virtual ~MeshRenderHandler() = default;

    private:
    MeshRenderHandler(QVTKRenderWindow* aRenderWindow, const ModelDataView& aModelDataView, QObject* aParent);

    public slots:

    void showMeshActor();

    private slots:


    private:

    QVTKRenderWindow* _renderWindow;
    const ModelDataView& _modelDataView;

};

}

#endif