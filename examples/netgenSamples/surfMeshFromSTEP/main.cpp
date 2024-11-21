#include <exception>
#include <iostream>
#include <occgeom.hpp>

#include "BRepGProp.hxx"
#include "GProp_GProps.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Shape.hxx"

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkCylinderSource.h>
#include <vtkDataSetMapper.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataWriter.h>
#include <vtkProperty.h>
#include <vtkQuad.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>

namespace nglib {
#include <nglib.h>
#include <nglib_occ.h>
}
using namespace nglib;

int main(int, char*[]) {

	std::cout << "Netgen NgLib - OpenCascade Test Case" << std::endl;

	Ng_OCC_Geometry* occ_geom;
	Ng_Mesh* occ_mesh;
	Ng_Meshing_Parameters mp;

	TopTools_IndexedMapOfShape FMap;
	Ng_OCC_TopTools_IndexedMapOfShape* occ_fmap = (Ng_OCC_TopTools_IndexedMapOfShape*)&FMap;
	Ng_Result ng_res;

	Ng_Init();

	std::string stepFilePath = "/home/pgilewicz/geometrySample/Ref_XYZ.stp";

	occ_geom = Ng_OCC_Load_STEP(stepFilePath.c_str());
	if (!occ_geom) {
		std::cout << "Error reading in STEP File: " << stepFilePath << std::endl;
		return 1;
	}
	std::cout << "Successfully loaded STEP File: " << stepFilePath << std::endl;

	occ_mesh = Ng_NewMesh();
	ng_res = Ng_OCC_GetFMap(occ_geom, occ_fmap);
	std::cout << "ng_res = " << ng_res << std::endl;

	if (!FMap.Extent()) {
		std::cout << "Error retrieving Face map...." << std::endl;
		return 1;
	}

	std::cout << "Successfully extracted the Face Map....:" << FMap.Extent() << std::endl;

	for (int i = 1; i <= FMap.Extent(); i++) {
		TopoDS_Face OCCface;
		OCCface = TopoDS::Face(FMap.FindKey(i));

		GProp_GProps faceProps;
		BRepGProp::SurfaceProperties(OCCface, faceProps);

		std::cout << "Index: " << i
				  << " :: Area: " << faceProps.Mass()
				  << " :: Hash: " << OCCface.HashCode(1e+6)
				  << std::endl;
	}

	mp.uselocalh = 1;
	// mp.elementsperedge = 2.0;
	// mp.elementspercurve = 2.0;
	mp.maxh = 25.0;
	mp.minh = 5.0;
	mp.grading = 0.9;
	mp.closeedgeenable = 1;
	mp.closeedgefact = 1.0;
	// mp.optsurfmeshenable = 1;
	mp.quad_dominated = 0;

	std::cout
		<< "Setting Local Mesh size....." << std::endl;
	std::cout << "OCC Mesh Pointer before call = " << occ_mesh << std::endl;
	Ng_OCC_SetLocalMeshSize(occ_geom, occ_mesh, &mp);
	std::cout << "Local Mesh size successfully set....." << std::endl;
	std::cout << "OCC Mesh Pointer after call = " << occ_mesh << std::endl;

	std::cout << "Creating Edge Mesh....." << std::endl;
	ng_res = Ng_OCC_GenerateEdgeMesh(occ_geom, occ_mesh, &mp);
	if (ng_res != NG_OK) {
		Ng_DeleteMesh(occ_mesh);
		std::cout << "Error creating Edge Mesh.... Aborting!!" << std::endl;
		return 1;
	} else {
		std::cout << "Edge Mesh successfully created....." << std::endl;
		std::cout << "Number of points = " << Ng_GetNP(occ_mesh) << std::endl;
	}

	std::cout << "Creating Surface Mesh....." << std::endl;

	ng_res = Ng_OCC_GenerateSurfaceMesh(occ_geom, occ_mesh, &mp);
	if (ng_res != NG_OK) {
		Ng_DeleteMesh(occ_mesh);
		std::cout << "Error creating Surface Mesh..... Aborting!!" << std::endl;
		return 1;
	} else {
		std::cout << "Surface Mesh successfully created....." << std::endl;
		std::cout << "Number of points = " << Ng_GetNP(occ_mesh) << std::endl;
		std::cout << "Number of surface elements = " << Ng_GetNSE(occ_mesh) << std::endl;
		Ng_SaveMesh(occ_mesh, "test_occ_surface.vol");
	}

	// std::cout << "Creating Volume Mesh....." << std::endl;

	// ng_res = Ng_GenerateVolumeMesh(occ_mesh, &mp);

	// std::cout << "Volume Mesh successfully created....." << std::endl;
	// std::cout << "Number of points = " << Ng_GetNP(occ_mesh) << std::endl;
	// std::cout << "Number of volume elements = " << Ng_GetNE(occ_mesh) << std::endl;

	// std::cout << "Saving Mesh as VOL file....." << std::endl;
	// Ng_SaveMesh(occ_mesh, "test_occ.vol");

	// Create VTK points and polygons containers
	auto vtk_points = vtkSmartPointer<vtkPoints>::New();
	auto vtkPolygons = vtkSmartPointer<vtkCellArray>::New();

	// Add Netgen points to VTK points
	double coords[3];
	for (int i = 1; i <= Ng_GetNP(occ_mesh); ++i) {
		Ng_GetPoint(occ_mesh, i, coords);
		// std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
		vtk_points->InsertNextPoint(coords[0], coords[1], coords[2]);
	}

	int vertexIndices[4];
	for (int i = 1; i <= Ng_GetNSE(occ_mesh); ++i) {
		Ng_Surface_Element_Type elementType = Ng_GetSurfaceElement(occ_mesh, i, vertexIndices);
		if (elementType == NG_TRIG) // Triangle element
		{
			auto triangle = vtkSmartPointer<vtkTriangle>::New();
			triangle->GetPointIds()->SetId(0, vertexIndices[0] - 1); // Netgen indices are 1-based, VTK is 0-based
			triangle->GetPointIds()->SetId(1, vertexIndices[1] - 1);
			triangle->GetPointIds()->SetId(2, vertexIndices[2] - 1);

			vtkPolygons->InsertNextCell(triangle);
		} else if (elementType == NG_QUAD) // Quad element
		{
			auto quad = vtkSmartPointer<vtkQuad>::New();
			quad->GetPointIds()->SetId(0, vertexIndices[0] - 1);
			quad->GetPointIds()->SetId(1, vertexIndices[1] - 1);
			quad->GetPointIds()->SetId(2, vertexIndices[2] - 1);
			quad->GetPointIds()->SetId(3, vertexIndices[3] - 1);

			vtkPolygons->InsertNextCell(quad);
		} else {
			std::cerr << "Unsupported element type encountered: " << elementType << std::endl;
		}
	}

	// Create PolyData and set points and polygons
	auto polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(vtk_points);
	polyData->SetPolys(vtkPolygons);

	// Create a mapper and actor
	auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(polyData);

	auto actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	actor->GetProperty()->SetEdgeVisibility(true);
	actor->GetProperty()->SetEdgeColor(0.0, 0.0, 0.0);
	actor->GetProperty()->SetLineWidth(1.5);

	vtkNew<vtkNamedColors> colors;
	vtkNew<vtkRenderer> renderer;
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
	renderer->ResetCamera();
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->SetSize(600, 600);
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Mesh Sample");
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	auto style = vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindow->Render();
	renderWindowInteractor->Start();

	Ng_Exit();

	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName("output.vtk"); // Specify the output file name
	writer->SetInputData(polyData);
	writer->Write();

	return EXIT_SUCCESS;
}
