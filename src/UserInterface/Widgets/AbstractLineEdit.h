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
 */

#ifndef ABSTRACTLINEEDIT_H
#define ABSTRACTLINEEDIT_H

#include "BaseWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QModelIndex>
#include <QValidator>
#include <QWidget>
#include <QXmlStreamAttributes>

class AbstractLineEdit : public BaseWidget {
	Q_OBJECT

public:
	AbstractLineEdit(QWidget* parent = nullptr, QValidator* validator = nullptr);
	~AbstractLineEdit();
	void initialize(const QModelIndex& index);
	void setValue(const std::string&);
	std::string getValue();

	static const int lineHeight = 20;
	static const int fontSize = 10;

protected:
	QLineEdit* m_lineEdit;
	QLabel* m_suffixLabel;
	double m_lastValidValue;
	double m_minVal, m_maxVal;

	QModelIndex m_index;
	QLocale m_locale;

	static QString labelStyleSheet;
	static QString lineEditStyleSheet;

signals:
	void valueChanged(QString);

public slots:
	void onEditingFinished();
};

#endif