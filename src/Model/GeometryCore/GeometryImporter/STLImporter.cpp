#include "STLImporter.hpp"
#include "ProgressEvent.hpp"
#include "ModelSubject.hpp"

void GeometryCore::STLImporter::import(const std::string& aFileName, const ModelSubject& aModelSubject){
	std::string actionLabel = "Importing STL from file: " + aFileName;
	ProgressEvent progressEvent("Importing STL geometry: " + aFileName, 0);
	aModelSubject.publishEvent(progressEvent);

	TCollection_AsciiString aName((Standard_CString)aFileName.data());
	OSD_Path aFile(aName);

	BRepBuilderAPI_Sewing shapeSewer;

	Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile(aFile);

	Standard_Integer numberOfTriangles = aSTLMesh->NbTriangles();

	TopoDS_Vertex Vertex1, Vertex2, Vertex3;
	TopoDS_Shape shape;
	TopoDS_Face face;
	TopoDS_Wire wire;

	for (Standard_Integer i = 1; i <= numberOfTriangles; i++) {
		int progress = static_cast<int>(100.0 * i / numberOfTriangles);
		progressEvent.value = progress;
		aModelSubject.publishEvent(progressEvent);

		Poly_Triangle triangle = aSTLMesh->Triangle(i);

		Standard_Integer n1;
		Standard_Integer n2;
		Standard_Integer n3;

		triangle.Get(n1, n2, n3);

		gp_Pnt p1 = aSTLMesh->Node(n1);
		gp_Pnt p2 = aSTLMesh->Node(n2);
		gp_Pnt p3 = aSTLMesh->Node(n3);

		if (!p1.IsEqual(p2, 0.0) && !p1.IsEqual(p3, 0.0)) {
			Vertex1 = BRepBuilderAPI_MakeVertex(p1);
			Vertex2 = BRepBuilderAPI_MakeVertex(p2);
			Vertex3 = BRepBuilderAPI_MakeVertex(p3);

			wire = BRepBuilderAPI_MakePolygon(Vertex1, Vertex2, Vertex3, Standard_True);
			if (!wire.IsNull()) {
				face = BRepBuilderAPI_MakeFace(wire);
				if (!face.IsNull()) {
					shapeSewer.Add(face);
				}
			}
		}
	}

	progressEvent.label = "Sewing faces...";
	progressEvent.value = 0;
	aModelSubject.publishEvent(progressEvent);

	shapeSewer.Perform();
	shape = shapeSewer.SewedShape();

	progressEvent.label = "Extracting shells...";
	aModelSubject.publishEvent(progressEvent);

	BRepBuilderAPI_MakeSolid solidmaker;
	TopTools_IndexedMapOfShape shellMap;
	TopExp::MapShapes(shape, TopAbs_SHELL, shellMap);

	unsigned int counter = 0;
	for (int ishell = 1; ishell <= shellMap.Extent(); ++ishell) {
		
		int progress = static_cast<int>(100.0 * ishell / shellMap.Extent());
		progressEvent.value = progress;
		aModelSubject.publishEvent(progressEvent);

		const TopoDS_Shell& shell = TopoDS::Shell(shellMap(ishell));
		solidmaker.Add(shell);
		counter++;
		std::string uniqueName = getUniqueObjectName("ShellShape", this->_shapesMap);
        // std::cout << "Added shell part: " << uniqueName << std::endl;
		this->_shapesMap[uniqueName] = shell;
	}

	std::cout << " -> shells found: " << counter << std::endl;

	progressEvent.label = "Converting to solid...";
	aModelSubject.publishEvent(progressEvent);

	TopoDS_Shape solid = solidmaker.Solid();

	std::cout << " -> done." << std::endl;

	progressEvent.value = 100;
	progressEvent.label = "Done.";
	aModelSubject.publishEvent(progressEvent);
}
