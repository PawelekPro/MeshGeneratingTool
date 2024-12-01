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

#ifndef RIBBONBARWIDGET_HPP
#define RIBBONBARWIDGET_HPP

#include "MainWindow.hpp"
#include "QIVtkViewRepresentation.h"
#include "SARibbon.h"

#include <QActionGroup>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class SARibbonBar;
class RibbonBarWidget : public QWidget {
	Q_OBJECT

public:
	RibbonBarWidget(QWidget* parent = nullptr);
	~RibbonBarWidget();
	void initialize() {
		_QVTKRender = _mainWindow->getRenderWindow();
	};

public:
	QVBoxLayout* verticalLayout;

private:
	void buildRibbon(SARibbonBar* bar);
	void setupUi();

	QAction* createAction(const QString& text, const QString& iconurl);

private slots:
	void onRibbonThemeComboBoxCurrentIndexChanged(int index);
	void onEntitySelectionChanged(QAction* action);
	void onViewRepresentationChanged(QAction* action);
	void onPressedSizeField(QAction* action);

private:
	MainWindow* _mainWindow;
	Rendering::QVTKRenderWindow* _QVTKRender;
	SARibbonBar* _ribbonBar { nullptr };
	QComboBox* mComboTheme { nullptr };
};

#endif