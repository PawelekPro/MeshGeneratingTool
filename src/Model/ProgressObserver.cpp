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

#include "ProgressObserver.hpp"
#include <algorithm>

void ProgressObserver::setObserverCallbacks(
    StartCallback aStartCallback,
    ProgressCallback aProgressCallback,
    FinishCallback aFinishCallback){
  _startCallback = std::move(aStartCallback);
  _progressCallback = std::move(aProgressCallback);
  _finishCallback = std::move(aFinishCallback);
}

int ProgressObserver::currentProgress(){
    return _currentProgress;
}

std::string ProgressObserver::currentLabel(){
    return _currentLabel;
}
void ProgressObserver::startNewOperation(const std::string& aLabel, int maxProgress){
    _currentLabel = aLabel; 
    _currentProgress = 0; 
    _maxProgress = maxProgress;
    _startCallback(aLabel, maxProgress);
}
void ProgressObserver::progressOperation(
    const std::string& aStepLabel,
    int progress
    ){
  _currentLabel = genProgressLabel(aStepLabel, progress);
  _currentProgress = std::min(progress, _maxProgress);
  _progressCallback(_currentLabel, _currentProgress);
}

void ProgressObserver::finishOperation(const std::string& aFinisLabel){
    _currentProgress = _maxProgress;
    _currentLabel = aFinisLabel;
    _finishCallback(_currentLabel);
}

std::string ProgressObserver::genProgressLabel(
    const std::string& aMessage,
    int progress
    ){
  std::string progressBraces = 
    "[ " + 
    std::to_string(progress) + "/" + std::to_string(_maxProgress) + 
    " ] - ";
  return progressBraces + aMessage;
} 