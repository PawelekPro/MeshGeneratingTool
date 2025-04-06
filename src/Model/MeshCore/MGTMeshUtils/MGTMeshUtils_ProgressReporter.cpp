/*
* Copyright (C) 2024 Paweł Gilewicz
 *
 * This file is part of the Mesh Generating Tool.
(https://github.com/PawelekPro/MeshGeneratingTool)
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

*=============================================================================
* File      : MGTMeshUtils_ViscousLayers.h
* Author    : Paweł Gilewicz
* Date      : 06/04/2025
*/
#include "MGTMeshUtils_ProgressReporter.hpp"

#include <iostream>

//----------------------------------------------------------------------------
MGTMeshUtils_ProgressReporter::MGTMeshUtils_ProgressReporter(int intervalMs,
	std::function<int()> computeProgress, std::function<void(int)> sendProgress)
	: _callback(std::move(sendProgress))
	, _computeProgress(std::move(computeProgress))
	, _stopFlag(false) {
	_thread = std::thread([this, intervalMs]() {
		while (!_stopFlag.load()) {
			const int progress = _computeProgress();
			CALL_PROGRESS(_callback, progress);
			std::cout << "blabla" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
		}
	});
}

//----------------------------------------------------------------------------
MGTMeshUtils_ProgressReporter::~MGTMeshUtils_ProgressReporter() {
	_stopFlag.store(true);
	if (_thread.joinable())
		_thread.join();
	std::cout << "mehmehmeh" << std::endl;
}