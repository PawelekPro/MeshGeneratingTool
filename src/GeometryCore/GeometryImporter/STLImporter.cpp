#include "STLImporter.h"

void GeometryCore::STLImporter::import(const std::string& fileName, QWidget* parent){
	ProgressBar* progressBar = qobject_cast<ProgressBar*>(parent);
	progressBar->initialize();
	progressBar->setProgressMessage("Converting to faces...");

	TCollection_AsciiString aName((Standard_CString)fileName.data());
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
		progressBar->setValue(progress);

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

	// progressBar->setValue(0);
	// progressBar->setProgressMessage("Sewing faces...");

	shapeSewer.Perform();
	shape = shapeSewer.SewedShape();

	// progressBar->setProgressMessage("Extracting shells...");

	BRepBuilderAPI_MakeSolid solidmaker;
	TopTools_IndexedMapOfShape shellMap;
	TopExp::MapShapes(shape, TopAbs_SHELL, shellMap);

	unsigned int counter = 0;
	for (int ishell = 1; ishell <= shellMap.Extent(); ++ishell) {
		// int progress = static_cast<int>(100.0 * ishell / shellMap.Extent());
		// progressBar->setValue(progress);

		const TopoDS_Shell& shell = TopoDS::Shell(shellMap(ishell));
		solidmaker.Add(shell);
		counter++;
		std::string uniqueName = getUniqueObjectName("ShellPart", this->_solidsMap);
        // std::cout << "Added shell part: " << uniqueName << std::endl;
		this->_solidsMap[uniqueName] = shell;
	}

	std::cout << " -> shells found: " << counter << std::endl;

	// progressBar->setProgressMessage("Converting to solid...");

	TopoDS_Shape solid = solidmaker.Solid();

	std::cout << " -> done." << std::endl;

	progressBar->finish();
}


const GeometryCore::ActorsMap GeometryCore::STLImporter::getPartsActorMap(){
	GeometryCore::ActorsMap actorsMap {};
	for (const auto& it : this->_solidsMap) {
		const auto& shape = it.second;
		vtkSmartPointer<vtkActor> actor = createVTKActor(shape);
		actorsMap[it.first] = actor;
	}
	return actorsMap;
}