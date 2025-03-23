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

#include "ModelDocParser.hpp"
#include "DocUtils.hpp"

#include "MGTMesh_Algorithm.hpp"

ModelDocParser::ModelDocParser(Model& aModel)
	: _model(aModel)
	, _doc(DocumentHandler::getInstance()) { }

void ModelDocParser::applyElementSizings() {
	QList<QDomElement> sizingElements
		= _doc.getSubElements(ItemTypes::Mesh::ElementSizing);
	for (auto sizingElem : sizingElements) {
		std::pair<std::vector<int>, double> sizing
			= parseElementSizing(sizingElem);
		// _model.addSizing(sizing.first, sizing.second);
	}
}

std::pair<std::vector<int>, double> ModelDocParser::parseElementSizing(
	const QDomElement& aSizingElement) {
	// TODO: All those error checks should be in propertyValue - here they are
	// redundant and clutter the code
	QString sizeString, tagsString, shapeTypeString;

	QDomElement elementSizeProp
		= Properties::getProperty(aSizingElement, "elementSize");
	QDomElement selectedTagsProp
		= Properties::getProperty(aSizingElement, "selectedTags");
	QDomElement selectionTypeProp
		= Properties::getProperty(aSizingElement, "selectionType");

	sizeString = Properties::getPropertyValue(elementSizeProp);
	tagsString = Properties::getPropertyValue(selectedTagsProp);
	shapeTypeString = Properties::getPropertyValue(selectionTypeProp);

	if (sizeString.isEmpty()) {
		qWarning() << "Element size is empty in "
				   << aSizingElement.attribute("name") << " skipping...";
	}
	if (tagsString.isEmpty()) {
		qWarning() << "selectedTags value is empty in "
				   << aSizingElement.attribute("name") << " skipping...";
	}
	if (shapeTypeString.isEmpty()) {
		qWarning() << "selectedTags value is empty in "
				   << aSizingElement.attribute("name") << " skipping...";
	}
	double size = sizeString.toDouble();
	std::vector<int> verticesTags;
	GeometryCore::EntityType selectionType = GeometryCore::EntityType::Vertex;
	if (shapeTypeString == "Vertex") {
		selectionType = GeometryCore::EntityType::Vertex;
	} else if (shapeTypeString == "Edge") {
		selectionType = GeometryCore::EntityType::Edge;
	} else if (shapeTypeString == "Face") {
		selectionType = GeometryCore::EntityType::Face;
	} else if (shapeTypeString == "Solid") {
		selectionType = GeometryCore::EntityType::Solid;
	} else {
		qWarning() << "Selection type: " << shapeTypeString
				   << " does not exist, assuming Vertex";
	}
	QStringList tagsList = tagsString.split(',', Qt::SkipEmptyParts);
	for (const QString& tagString : tagsList) {
		int shapeTag = tagString.toInt();
		const TopoDS_Shape& shape
			= _model.geometry.getTagMap().getShape(selectionType, shapeTag);
		std::vector<int> shapeVerticesTags
			= _model.geometry.getShapeVerticesTags(shape);
		verticesTags.insert(verticesTags.end(), shapeVerticesTags.begin(),
			shapeVerticesTags.end());
	}
	return std::pair<std::vector<int>, double>(verticesTags, size);
}

void ModelDocParser::applyMeshSettings() {
	double minSize;
	double maxSize;
	QDomElement meshElement = _doc.getRootElement(ItemTypes::Root::Mesh);
	QDomElement property
		= Properties::getProperty(meshElement, "minElementSize");
	QString minValue = Properties::getPropertyValue(property);
	minSize = minValue.toDouble();
	if (minValue.isEmpty()) {
		qWarning()
			<< "Could not parse minElementSize property - setting to default 1";
		minSize = 0.1;
	} else {
		minSize = minValue.toDouble();
	}

	property = Properties::getProperty(meshElement, "maxElementSize");
	QString maxValue = Properties::getPropertyValue(property);
	if (maxValue.isEmpty()) {
		qWarning()
			<< "Could not parse maxElementSize property - setting to default 1";
		maxSize = 1.0;
	} else {
		maxSize = maxValue.toDouble();
	}

	std::cout << "settting sizes: " << minSize << " " << maxSize;
}

//----------------------------------------------------------------------------
std::unique_ptr<MGTMesh_Algorithm> ModelDocParser::generateMeshAlgorithm(
	const bool surfaceMesh) const {

	const QMap<QString, QString> propMap
		= _doc.getPropertyNodeMap(ItemTypes::Root::Mesh);

	for (const QString& propName : propMap.keys()) {
		std::cout << propName.toStdString() << " : "
				  << propMap.value(propName).toStdString() << std::endl;
	}

	int schemeId = 0;
	auto algorithm = std::make_unique<MGTMesh_Algorithm>(schemeId);
	algorithm->SetSchemeName(propMap.value("algName").toStdString());

	const std::unordered_map<QString, std::function<void(const QString&)>>
		setters = {
			{ "elementsOrder",
				[&](const QString& v) {
					algorithm->secondOrder = v.toInt() != 0;
				} },
			{ "globalFineness",
				[&](const QString& v) {
					algorithm->fineness
						= static_cast<MGTMesh_MeshParameters::Fineness>(
							v.toInt());
				} },
			{ "quadDominated",
				[&](const QString& v) {
					algorithm->quadAllowed = v.toInt() != 0;
				} },
			{ "limitSizeBySurfaceCurvature",
				[&](const QString& v) {
					algorithm->surfaceCurvature = v.toInt() != 0;
				} },
			{ "maxElementSize",
				[&](const QString& v) { algorithm->maxSize = v.toDouble(); } },
			{ "minElementSize",
				[&](const QString& v) { algorithm->minSize = v.toDouble(); } },
			{ "growthRate",
				[&](const QString& v) {
					algorithm->growthRate = v.toDouble();
				} },
			{ "nbPerRadius",
				[&](const QString& v) {
					algorithm->nbSegPerRadius = v.toDouble();
				} },
			{ "nbPerEdge",
				[&](const QString& v) {
					algorithm->nbSegPerEdge = v.toDouble();
				} },
			{ "optimizeMesh",
				[&](const QString& v) {
					algorithm->optimize = v.toInt() != 0;
				} },
		};

	for (auto it = propMap.constBegin(); it != propMap.constEnd(); ++it) {
		if (auto setter = setters.find(it.key()); setter != setters.end()) {
			setter->second(it.value());
		}
	}

	if (!surfaceMesh)
		algorithm->SetType(MGTMesh_Scheme::ALG_3D);

	return algorithm;
}