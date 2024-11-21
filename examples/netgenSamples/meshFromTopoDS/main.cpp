#include <exception>
#include <iostream>

#include "BRepGProp.hxx"
#include "GProp_GProps.hxx"
#include "TopTools_IndexedMapOfShape.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Shape.hxx"
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools.hxx>
#include <Bnd_Box.hxx>

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

#ifndef OCCGEOMETRY
#define OCCGEOMETRY
#endif
#include <meshing.hpp>
#include <occgeom.hpp>

using namespace nglib;

namespace netgen {
MeshingParameters mparam;
}

void PrepareOCCGeometry(netgen::OCCGeometry& occgeo, const TopoDS_Shape& shape) {
	occgeo.shape = shape;
	occgeo.changed = 1;
	occgeo.BuildFMap();
	occgeo.BuildVisualizationMesh(netgen::mparam.minh);
	occgeo.CalcBoundingBox();
	occgeo.PrintNrShapes();
}

namespace {
inline void NOOP_Deleter(void*) { ; }
}

int main(int, char*[]) {

	std::cout << "Netgen NgLib - OpenCascade Test Case" << std::endl;
	netgen::mparam.maxh = 2.5;
	netgen::mparam.minh = 1;
	netgen::mparam.grading = 0.3;
	netgen::mparam.quad = true;
	netgen::mparam.uselocalh = true;
	netgen::mparam.secondorder = false;

	netgen::OCCParameters occParam;

	Ng_Init();

	double length = 100.0, width = 100.0, height = 50.0;
	TopoDS_Shape box = BRepPrimAPI_MakeBox(length, width, height).Shape();
	netgen::OCCGeometry occgeo;
	PrepareOCCGeometry(occgeo, box);

	netgen::Mesh ngMesh;
	netgen::MeshingParameters& mparams = netgen::mparam;

	ngMesh.SetGeometry(std::shared_ptr<netgen::NetgenGeometry>(&occgeo, &NOOP_Deleter));

	netgen::OCCSetLocalMeshSize(occgeo, ngMesh, mparams, occParam);
	occgeo.FindEdges(ngMesh, mparams);
	occgeo.MeshSurface(ngMesh, mparams);

	const int nbNodes = (int)ngMesh.GetNP();
	const int nbTriangles = (int)ngMesh.GetNSE();

	std::cout << "Num. of mesh nodes     generated: " << nbNodes << std::endl;
	std::cout << "Num. of mesh triangles generated: " << nbTriangles << std::endl;

	if (!nbNodes || !nbTriangles) {
		nglib::Ng_Exit();
		return false;
	}

	Ng_Exit();

	// Create VTK points and polygons containers
	auto vtk_points = vtkSmartPointer<vtkPoints>::New();
	auto vtkPolygons = vtkSmartPointer<vtkCellArray>::New();

	// Add Netgen points to VTK points

	for (int i = 1; i <= nbNodes; ++i) {
		const netgen::MeshPoint& mp = ngMesh.Point(i);
		double coords[3] = { mp(0), mp(1), mp(2) };
		// std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
		vtk_points->InsertNextPoint(coords[0], coords[1], coords[2]);
	}

	for (int i = 1; i <= nbTriangles; ++i) {
		const netgen::Element2d& elem = ngMesh.SurfaceElement(i);
		int elementType = elem.GetType();

		const auto vertices = elem.PNums();

		if (elementType == netgen::TRIG) // Triangle element
		{
			auto triangle = vtkSmartPointer<vtkTriangle>::New();
			triangle->GetPointIds()->SetId(0, vertices[0] - 1); // Netgen indices are 1-based, VTK is 0-based
			triangle->GetPointIds()->SetId(1, vertices[1] - 1);
			triangle->GetPointIds()->SetId(2, vertices[2] - 1);

			vtkPolygons->InsertNextCell(triangle);
		} else if (elementType == netgen::QUAD) // Quad element
		{
			auto quad = vtkSmartPointer<vtkQuad>::New();
			quad->GetPointIds()->SetId(0, vertices[0] - 1);
			quad->GetPointIds()->SetId(1, vertices[1] - 1);
			quad->GetPointIds()->SetId(2, vertices[2] - 1);
			quad->GetPointIds()->SetId(3, vertices[3] - 1);

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

	// auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	// writer->SetFileName("output.vtk"); // Specify the output file name
	// writer->SetInputData(polyData);
	// writer->Write();

	return EXIT_SUCCESS;
}
