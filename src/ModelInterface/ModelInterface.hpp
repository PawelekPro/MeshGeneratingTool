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

// TODO: Split this logic into GeometryInterface when the Model itsefl goes into rework

#ifndef MODELINTERFACE_HPP
#define MODELINTERFACE_HPP

#include "ModelManager.hpp"
#include <QObject>

#include "ModelManager.hpp"
#include <QObject>
#include <QString>
#include <QWidget>
#include <memory>

class ModelInterface : public QObject {
    Q_OBJECT

public:
    explicit ModelInterface(ModelManager& modelManager, QObject* parent = nullptr)
        : QObject(parent), _modelManager(modelManager) {}

signals:
    void someSignal();

public slots:
    void importSTEP(const QString& filePath, QWidget* progressBar);
    void meshSurface();
    void meshVolume();


private:
    ModelManager& _modelManager;
};

#endif