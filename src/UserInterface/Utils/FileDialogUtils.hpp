/*
 * Copyright (C) 2024 Krystian Fudali
 *
 * This file is part of the Mesh Generating Tool.
 * (https://github.com/PawelekPro/MeshGeneratingTool)
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

#ifndef FILEDIALOGUTILS_HPP
#define FILEDIALOGUTILS_HPP

#include <QFileDialog>
#include <QWidget>
#include <functional>

/**
 * @brief namespace for functions that create filedialog window
 */
namespace FileDialogUtils {
constexpr auto FilterSTEP = "STEP Files (*.step *.stp)";
constexpr auto FilterSTL = "STL Files (*.stl)";
constexpr auto FilterAll = "All Files (*)";

/**
 * @brief Creates a filedialog window and retruns path to file selected by the
 * user.
 * @param actionName - name of the filedialog window that will be displayed in
 * top bar.
 * @param filter - file extension filter - use FileDialogUtils::filterName.
 * @param parent - parent that will be assigned to filedialog window.
 */
QString getFileSelection(const QString& actionName, const QString& filter,
	QWidget* parent = nullptr);

/**
 * @brief Creates a filedialog window and calls action with the selected file
 * path as an argument
 * @param action - callable with QString argument that will be called upon
 * confirming file selection
 * @param actionName - name of the filedialog window that will be displayed in
 * top bar.
 * @param filter - file extension filter - use FileDialogUtils::filterName.
 * @param parent - parent that will be assigned to filedialog window.
 */
int executeWithFileSelection(const std::function<void(QString)>& action,
	const QString& actionName, const QString& filter,
	QWidget* parent = nullptr);
}
#endif
