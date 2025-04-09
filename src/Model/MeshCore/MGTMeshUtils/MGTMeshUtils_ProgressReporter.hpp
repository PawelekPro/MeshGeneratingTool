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
#ifndef MGTMESHUTILS_PROGRESSREPORTER_HPP
#define MGTMESHUTILS_PROGRESSREPORTER_HPP
#include <functional>
#include <thread>

/* clang-format off */
#define CALL_STATUS(cb, msg) \
do { if (cb) cb(msg); } while (0)

#define CALL_PROGRESS(cb, val) \
do { if (cb) cb(val); } while (0)
/* clang-format on */

// Smart RAII guard with custom deleter to ensure reporter stops
class MGTMeshUtils_ProgressReporter {
public:
	MGTMeshUtils_ProgressReporter(int intervalMs,
		std::function<int()> computeProgress,
		std::function<void(int)> sendProgress);

	~MGTMeshUtils_ProgressReporter();

private:
	std::thread _thread;
	std::function<void(int)> _progressCallback;
	std::function<int()> _computeProgress;
	std::atomic<bool> _stopFlag;
};

#endif
