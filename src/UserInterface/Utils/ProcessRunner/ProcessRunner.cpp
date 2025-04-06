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
* File      : ProcessRunner.cpp
* Author    : Paweł Gilewicz
* Date      : 06/04/2025
*/
#include "ProcessRunner.hpp"

//----------------------------------------------------------------------------
ProcessRunner::ProcessRunner(QObject* parent)
	: QThread(parent)
	, stopRequested(false) { }

//----------------------------------------------------------------------------
void ProcessRunner::stop() {
	stopRequested.store(true);
	emit statusMessage("Stopping process...");
}

//----------------------------------------------------------------------------
bool ProcessRunner::isStopRequested() const { return stopRequested.load(); }

//----------------------------------------------------------------------------
void ProcessRunner::run() {
	try {
		if (task) {
			task();
			emit finishedSuccessfully();
		}
	} catch (const std::exception& e) {
		emit failed("Error: " + std::string(e.what()));
	} catch (...) {
		emit failed("Unknown error occurred.");
	}
}