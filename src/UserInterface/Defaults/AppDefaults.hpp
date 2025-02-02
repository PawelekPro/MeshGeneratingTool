/*
 * Copyright (C) 2024 Paweł Gilewicz
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

*=============================================================================
* File      : AppDefaults.hpp
* Author    : Paweł Gilewicz
* Date      : 06/01/2025
*/
#ifndef APPDEFAULTS_H
#define APPDEFAULTS_H

#include "AppDefaultColors.hpp"
#include "AppSettings.hpp"

class AppDefaults : public AppSettings {
public:
	/**
	 * @brief Provides a global point of access to the singleton instance of the AppDefaults class.
	 *
	 * This static member function follows the Singleton design pattern to ensure that only one instance
	 * of the AppDefaults class is created throughout the application's lifetime. It returns a reference
	 * to the singleton instance. If the instance does not exist yet, it creates one. Subsequent calls
	 * to this function return the same instance.
	 *
	 * @return A reference to the singleton instance of the AppDefaults class.
	 */
	static AppDefaults& getInstance() {
		static AppDefaults instance;
		return instance;
	};

private:
	AppDefaults(AppDefaults const&) = delete;
	AppDefaults() = default;
	~AppDefaults() = default;
	void operator=(AppDefaults const&) = delete;
};

#endif