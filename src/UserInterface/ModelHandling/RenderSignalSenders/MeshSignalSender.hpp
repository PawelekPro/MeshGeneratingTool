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

#ifndef MESHSIGNALSENDER_HPP
#define MESHSIGNALSENDER_HPP

#include <QObject>

/**
 * Class that serves as a collection of signals to handle renderer upon mesh
 * generation/modifiaction. To implement new  render interaction create signal here, implement slot
 * in MeshSignalHandler and connect them in mainwindow.
 */
class MeshSignalSender : public QObject {
	Q_OBJECT

public:
	MeshSignalSender(QObject* aParent);

signals:

	/**
	 * Signal to be sent when the model generates a mesh.
	 */
	void meshGenerated();
};

#endif