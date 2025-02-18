/*
 * Copyright (C) 2024 Krystian Fudali
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

#include "STLOcafImporter.hpp"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <RWStl.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Wire.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include "BrepTools.hpp"

STLOcafImporter::STLOcafImporter(const IEventSubject& aModelSubject) : 
    OcafImporter(aModelSubject),
    _progress(aModelSubject, "Importing STL..."){};



bool STLOcafImporter::importToDocument(
    const std::string& aFilePath, 
    Handle(TDocStd_Document) aDestDoc){
  
	std::string label = "Importing STL from file: " + aFilePath;
    _progress.publishProgress(label, 0);

	TCollection_AsciiString aName((Standard_CString)aFilePath.data());
	OSD_Path aFile(aName);
	Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile(aFile, _progress.Start());

	TopoDS_Shape shape = sewShape(aSTLMesh);
	shape = extractShells(shape);
	bool result = transferShape(shape, aDestDoc);
	_progress.publishProgress("Done.", 100);

    return result;
}

bool STLOcafImporter::transferShape(
    const TopoDS_Shape& aShape, 
    Handle(TDocStd_Document) aDestDoc) {

    if (aShape.IsNull()) {
        std::cerr << "Could not import STL - null shape" << std::endl;
        return false;
    }

    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(aDestDoc->Main());
    if (shapeTool.IsNull()) {
        std::cerr << "Document does not support shapes." << std::endl;
        return false;
    }

    TDF_Label shapeLabel = shapeTool->NewShape();
    shapeTool->SetShape(shapeLabel, aShape);
    TDataStd_Name::Set(shapeLabel, TCollection_ExtendedString("STLShape"));

    return true;
}


TopoDS_Shape STLOcafImporter::extractShells(const TopoDS_Shape& aShape){
	std::string label = "Extracting shells...";
    _progress.publishProgress(label, 0);
	BRepBuilderAPI_MakeSolid solidmaker;
	TopTools_IndexedMapOfShape shellMap;
	TopExp::MapShapes(aShape, TopAbs_SHELL, shellMap);
	int nShell = 0;
	std::for_each(
		shellMap.cbegin(), shellMap.cend(), 
		[&, nShell](const TopoDS_Shape& shape) mutable {
			int progress = static_cast<int>(100.0 * nShell / shellMap.Extent());
			_progress.publishProgress(label, progress);

			const TopoDS_Shell& shell = TopoDS::Shell(shape);
			solidmaker.Add(shell);
			nShell++;
		}
	);
	TopoDS_Shape solid = solidmaker.Solid();
	return solid;
}


TopoDS_Shape STLOcafImporter::sewShape(Handle(Poly_Triangulation) aTriangulation){
	
	const Poly_Array1OfTriangle& triangles = aTriangulation->Triangles();
    BRepBuilderAPI_Sewing shapeSewer;

	std::for_each(triangles.begin(), triangles.end(), [&](const Poly_Triangle& triangle) {
		Standard_Integer n1, n2, n3;
		triangle.Get(n1, n2, n3);

		const gp_Pnt& p1 = aTriangulation->Node(n1);
		const gp_Pnt& p2 = aTriangulation->Node(n2);
		const gp_Pnt& p3 = aTriangulation->Node(n3);

		shapeSewer.Add(BrepTools::makeTriangleFace(p1, p2, p3));
	});

    _progress.publishProgress("Sewing shapes", 0);
	shapeSewer.Perform(_progress.Start());
	TopoDS_Shape shape = shapeSewer.SewedShape();
	return shape;
}