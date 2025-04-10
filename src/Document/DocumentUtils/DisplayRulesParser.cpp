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
* File      : DisplayRulesParser.hpp
* Author    : Paweł Gilewicz
* Date      : 09/03/2025
*/

#include "DisplayRulesParser.hpp"

#include <QDomDocument>
#include <QFile>

#include <spdlog/spdlog.h>

//-----------------------------------------------------------------------------
QMap<QString, QStringList> DisplayRulesParser::parseVisibilityRules(
	const QString& filePath) {
	QMap<QString, QStringList> visibilityRules;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		SPDLOG_WARN("Can not find file: {}", filePath.toStdString().data());
		return visibilityRules;
	}

	QDomDocument doc;
	if (!doc.setContent(&file)) {
		SPDLOG_ERROR("ERROR parsing file: {}", filePath.toStdString().data());
		return visibilityRules;
	}

	const QDomElement root = doc.documentElement();
	const QDomNodeList ruleNodes = root.elementsByTagName("Rule");

	for (int i = 0; i < ruleNodes.count(); ++i) {
		QDomElement ruleElement = ruleNodes.at(i).toElement();
		QString trigger = ruleElement.attribute("trigger");

		QDomNodeList hideNodes = ruleElement.elementsByTagName("Hide");
		QStringList hiddenElements;
		for (int j = 0; j < hideNodes.count(); ++j) {
			QDomElement hideElement = hideNodes.at(j).toElement();
			hiddenElements.append(hideElement.attribute("name"));
		}

		if (!trigger.isEmpty()) {
			visibilityRules.insert(trigger, hiddenElements);
		}
	}

	return visibilityRules;
}