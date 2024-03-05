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

#include "ProgressBar.h"

//----------------------------------------------------------------------------
ProgressBar::ProgressBar(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::ProgressBar) {
	ui->setupUi(this);

	this->ui->progressBar->setMinimum(0);
	this->ui->progressBar->setMaximum(100);

	connect(this->ui->stopButton, &QPushButton::clicked, this, &ProgressBar::handleStopButtonClicked);

	this->hide();
}

//----------------------------------------------------------------------------
ProgressBar::~ProgressBar() {
	delete ui;
}

void ProgressBar::initialize() {
	this->show();
	// fix for button not being displayed
	this->ui->stopButton->show();
	setValue(0);
}

//----------------------------------------------------------------------------
void ProgressBar::setValue(const int value) {
	this->ui->progressBar->setValue(value);
}

//----------------------------------------------------------------------------
void ProgressBar::setProgressMessage(const std::string text) {
	this->ui->message->setText(QString::fromStdString(text));
}

//----------------------------------------------------------------------------
void ProgressBar::finish() {
	this->hide();
}

//----------------------------------------------------------------------------
void ProgressBar::setMaximum(const int max) {
	this->ui->progressBar->setMaximum(max);
}

//----------------------------------------------------------------------------
void ProgressBar::setMinimum(const int min) {
	this->ui->progressBar->setMinimum(min);
}

//----------------------------------------------------------------------------
void ProgressBar::handleStopButtonClicked() {
	auto message = "Process aborted by the user.";
	vtkLogF(ERROR, message);
	this->_terminate = true;
}

//----------------------------------------------------------------------------
bool ProgressBar::getTerminateIndicator() {
	return this->_terminate;
}

//----------------------------------------------------------------------------
void ProgressBar::setTerminateIndicator(const bool sigTerm) {
	this->_terminate = sigTerm;
}
