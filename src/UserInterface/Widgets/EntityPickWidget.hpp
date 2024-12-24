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

#ifndef ENTITYPICKWIDHET_HPP
#define ENTITYPICKWIDHET_HPP

#include "BaseWidget.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QString>
#include <QWidget>

class GeometrySignalSender;
class PropertiesModel;

/**
 * @brief widget EntityPickWidget is used to pass ids of seleceted geometry shapes to corresponding model
 *  	  it has access to geometrySignals through propertiesModel->parent(treeStructure)->geometrySignal
 *        therefore forces its model to have treestructure as its parent. Widget has a line view and a button.
 * 	      When the button is pressed the ids of currently selected geometry shapes will be passed to model.
 */
class EntityPickWidget : public BaseWidget {
	Q_OBJECT
public:
	explicit EntityPickWidget(QWidget* parent);
	~EntityPickWidget();

	/**
	 * @brief Assigns index from propertiesModel to itself, so that the widget can call 
	 *  	  propertiesModel->setData(this->index). Also assigns _signalSender and _propModel.
	 */
	void setIndex(const QModelIndex& index) override;
	
	/**
	 * @brief Sets the state and display of button "Selecetd".
	 */
	void setSelected(bool selected);

private:
	
	/**
	 * @brief Updates the widget appearance by hiding/showing the button.
	 */
	void updateAppearance();

	QLabel* _selectionLabel;
	QPushButton* _selectionButton;
	QModelIndex _index;
	PropertiesModel* _propModel;
	GeometrySignalSender* _signalSender;

	static const int buttonWidth = 60;
	bool _selected;

protected:
	/**
	 * @brief Updates the widget appearance when mouse is pressed inside it - shows the "Selected" button.
	 */
	void mousePressEvent(QMouseEvent* event) override;

signals:
	/**
	 * @brief Signal triggered when the "Selected" button is pressed.
	 */
	void confirmed(const QString& selection);

private slots:

	/**
	* @brief Triggered when the button is pressed, updates the selected shape tags in the model and calls
	* 		 updateAppearance.
	*/
	void confirmSelection();
};

#endif