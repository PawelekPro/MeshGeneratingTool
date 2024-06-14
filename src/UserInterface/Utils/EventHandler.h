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

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <iostream>

#include "Model.h"
#include "QVTKRenderWindow.h"
#include "TreeWidgetEventHandler.h"
#include "DocumentHandler.h"

class EventHandler : public QObject {
    Q_OBJECT

    public:
        EventHandler(Model* model,
                     TreeWidgetEventHandler* treeEventHandler,
                     QVTKInteractorStyle* interactor,
                     DocumentHandler* documentHandler);

        /**
         * @brief  Connects various Qt slots with signals.
         *
         */
        void connectEvents();

    //Add compontents that require handling as private below and add them the constructor
    private:
        /**
         * @brief  Emits signal with selected shapes names back to EntityPickWidget so that the names can be displayed
         */
        void emitNamesSignal();

        /**
         * Raw pointer to model that contains Mesh and Geometry objects. Serves so that its data can be used in widgets
         */
        Model* _model;
        /**
	     * Raw pointer to event hadler for widgets inside Tree Widget Items.
	     */
        TreeWidgetEventHandler* _treeEventHandler;
         /**
	     * Raw pointer to interactor that handles seleceted shapes
	     */
        QVTKInteractorStyle* _interactor;
        /**
	     * Taw pointert to global document hadnler
	     */
        DocumentHandler* _documentHandler;

    signals:


    public slots:

};


#endif