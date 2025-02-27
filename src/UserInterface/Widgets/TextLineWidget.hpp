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
* File      : TextLineWidget.hpp
* Author    : Paweł Gilewicz
* Date      : 27/02/2025
*/
#ifndef TEXTLINEWIDGET_HPP
#define TEXTLINEWIDGET_HPP

#include "BaseWidget.hpp"

#include <QLabel>
#include <QModelIndex>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>

class TextLineWidget final : public BaseWidget {
    Q_OBJECT
public:
    explicit TextLineWidget(QWidget* parent = nullptr);

    ~TextLineWidget() override;

    void setIndex(const QModelIndex& index) override;

private:
	QLineEdit* _lineEdit;
    QModelIndex _index;
    QVBoxLayout* _layout;

private slots:
	void onEditingFinished() const;

};

#endif