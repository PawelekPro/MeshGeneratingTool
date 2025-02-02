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

#ifndef PROGRESSOBSERVER_HPP
#define PROGRESSOBSERVER_HPP

#include <string>
#include <functional>

class ProgressObserver {

    using StartCallback = std::function<void(const std::string&, int)>;
    using ProgressCallback = std::function<void(const std::string&, int)>;
    using FinishCallback = std::function<void(const std::string&)>;

    public:
        void setObserverCallbacks(
            StartCallback aStartCallback,
            ProgressCallback aProgressCallback,
            FinishCallback aFinishCallback
        );

        void startNewOperation(const std::string& aStartLabel, int maxProgress);
        void finishOperation(const std::string& aFinisLabel);
        void progressOperation(const std::string& aStepLabel, int progress);

        int currentProgress();
        std::string currentLabel(); 

    private:
        int _currentProgress = 0;
        int _maxProgress = 100;
        std::string _currentLabel;

        std::string genProgressLabel(
            const std::string& aLabel,
            int progress
        );

        StartCallback _startCallback = [](const std::string&) {};
        ProgressCallback _progressCallback = [](const std::string&, int) {};
        FinishCallback _finishCallback = [](const std::string&) {};
};  


#endif