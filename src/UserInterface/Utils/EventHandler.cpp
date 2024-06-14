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

#include "EventHandler.h"


EventHandler::EventHandler( Model* model,
                            TreeWidgetEventHandler* treeEventHandler,
                            QVTKInteractorStyle* interactor) : 
                            _model(model),
                            _treeEventHandler(treeEventHandler),
                            _interactor(interactor){
                                if(!model){
                                    qWarning("Model not initialized before setting event handler");
                                }
                                if(!treeEventHandler){
                                    qWarning("TreeEventHandler not initialized before setting event handler");
                                }
                                if(!interactor){
                                    qWarning("Interactor not initialized before setting event handler");
                                }
                                this->connectEvents();
                            }
                            

void EventHandler::connectEvents(){
    connect(_treeEventHandler, &TreeWidgetEventHandler::entitySelectionConfirmed,
            this, &EventHandler::emitNamesSignal);
}
void EventHandler::emitNamesSignal(){
    std::vector<std::string> selectedNames;
    std::vector<int> selectedTags;
    selectedNames = this->_model->geometry.getShapesNames(this->_interactor->getSelectedShapes());
    selectedTags = this->_model->geometry.getShapesVerticesTags(this->_interactor->getSelectedShapes());
    emit this->_treeEventHandler->sendNamesToWidgetDisplay(selectedNames, selectedTags);
}

