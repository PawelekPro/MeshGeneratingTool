/*
 * Copyright (C) 2024 Paweł Gilewicz, Krystian Fudali
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

#ifndef APPTHEME_H
#define APPTHEME_H
#include <QtAdvancedStylesheet.h>
#include <iostream>

#include <QApplication>
#include <QObject>

#include "AppSettings.hpp"

/**
 * @class AppTheme
 * @brief A singleton class for managing the application's stylesheet.
 *
 * The AppTheme class inherits from acss::QtAdvancedStylesheet and ensures
 * that only one instance of the theme manager exists throughout the
 * application's lifecycle. It provides a method to update the application's
 * stylesheet.
 */
class AppTheme : public acss::QtAdvancedStylesheet {
public:
	/**
	 * @brief Gets the singleton instance of the AppTheme class.
	 *
	 * This method returns the single instance of the AppTheme class. The first
	 * call to this method initializes the instance with the provided parent.
	 * Subsequent calls return the already initialized instance and ignore the
	 * parent parameter.
	 *
	 * @param parent The parent QObject for the AppTheme instance. Default is nullptr.
	 * @return AppTheme& Reference to the singleton instance.
	 */
	static AppTheme& getInstance(QObject* parent = nullptr) {
		static AppTheme instance(parent);
		return instance;
	}

	/**
	 * @brief Updates the application's stylesheet.
	 *
	 * This method should contain the implementation for updating the
	 * application's stylesheet.
	 */
	void updateAppStylesheet();

	void initializeAppStylesheet();

private:
	/**
	 * @brief Deleted copy constructor.
	 *
	 * The copy constructor is deleted to ensure that the singleton instance
	 * cannot be copied.
	 */
	AppTheme(const AppTheme&) = delete;

	/**
	 * @brief Construct an AppTheme object.
	 *
	 * This constructor is private to prevent direct instantiation. It initializes
	 * the AppTheme object with the provided parent.
	 *
	 * @param parent The parent QObject for the AppTheme instance. Default is nullptr.
	 */
	explicit AppTheme(QObject* parent = nullptr);

	/**
	 * @brief Destruct the AppTheme object.
	 *
	 * The destructor is defaulted. Since this is a singleton class, the destructor
	 * will only be called once when the application exits.
	 */
	~AppTheme() = default;
	/**
	 * @brief Deleted assignment operator.
	 *
	 * The assignment operator is deleted to ensure that the singleton instance
	 * cannot be assigned.
	 */
	AppTheme& operator=(const AppTheme&) = delete;

private:
	QObject* _parent;
};

#endif