#include <filesystem>
#include <iostream>
#include <vector>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Sphere.hxx>

#include <Poly_Polygon3D.hxx>
#include <Poly_Triangulation.hxx>

#include <TopTools_HSequenceOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_TangentialDeflection.hxx>
#include <Standard_TypeDef.hxx>
#include <TopExp_Explorer.hxx>

// vtk lib
#include <vtkActor.h>
#include <vtkAutoInit.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include <vtkAxes.h>
#include <vtkAxesActor.h>
#include <vtkAxis.h>
#include <vtkCaptionActor2D.h>
#include <vtkConeSource.h>
#include <vtkLineSource.h>

#include <vtkActorCollection.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkTriangle.h>

// vtkTransform, vtkTransformFilter, vtkMatrix4x4
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

// VTK includes
#include <vtkActor.h>
#include <vtkAxes.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLogger.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

// VIS includes
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapeObject.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>

// OCCT includes
#include <BRepPrimAPI_MakeBox.hxx>
#include <STEPControl_Reader.hxx>
#include <TColStd_MapIteratorOfPackedMapOfInteger.hxx>
#include <TColStd_PackedMapOfInteger.hxx>

class InteractorStylePick : public vtkInteractorStyleTrackballCamera {
public:
	static InteractorStylePick* New();
	vtkTypeMacro(InteractorStylePick, vtkInteractorStyleTrackballCamera);

	// Customization:
public:
	void SetRenderer(const vtkSmartPointer<vtkRenderer>& theRenderer) { m_renderer = theRenderer; }
	vtkSmartPointer<vtkRenderer> GetRenderer() const { return m_renderer; }
	void SetPicker(const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker) { m_picker = thePicker; }
	vtkSmartPointer<IVtkTools_ShapePicker> GetPicker() const { return m_picker; }

public:
	virtual void OnMouseMove() override {
		// Set selection mode only here in order to have access to rendered actors
		m_picker->SetSelectionMode(SM_Face);

		// Invoke basic method
		vtkInteractorStyleTrackballCamera::OnMouseMove();

		Standard_Integer aPos[2] = { this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1] };
		m_picker->Pick(aPos[0], aPos[1], 0);
		vtkActorCollection* anActorCollection = m_picker->GetPickedActors();
		IVtk_ShapeIdList ids = m_picker->GetPickedShapesIds();
		std::cout << ids.Size() << std::endl;
	}

private:
	InteractorStylePick(const InteractorStylePick&);
	void operator=(const InteractorStylePick&);
	InteractorStylePick()
		: vtkInteractorStyleTrackballCamera() { }
	~InteractorStylePick() { }
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<IVtkTools_ShapePicker> m_picker;
};

bool fileExists(const std::string& filename) {
	return std::filesystem::exists(filename);
}

TopoDS_Shape readSTEP(Standard_CString filePath) {
	STEPControl_Reader aReader;
	TopoDS_Shape aShape;
	IFSelect_ReturnStatus statSTEP = aReader.ReadFile(filePath);

	Standard_Integer nbr = aReader.NbRootsForTransfer();

	for (Standard_Integer n = 1; n <= nbr; n++) {
		std::cout << "STEP: Transferring Root " << n << std::endl;
		aReader.TransferRoot(n);
	}

	Standard_Integer nbs = aReader.NbShapes();
	if (nbs == 0) {
		throw std::runtime_error("No shapes found in file");
	} else {
		for (Standard_Integer i = 1; i <= nbs; i++) {
			aShape = aReader.Shape(i);
		}
	}

	return aShape;
}

vtkNew<vtkActor> createVTKActor(const TopoDS_Shape& shape) {
	IVtkOCC_Shape* vtkShapeAdapter = new IVtkOCC_Shape(shape);
	auto dataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
	dataSource->SetShape(vtkShapeAdapter);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(dataSource->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	return actor;
}

vtkStandardNewMacro(InteractorStylePick);

int main(int argc, char* argv[]) {

	vtkNew<vtkNamedColors> colors;
	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("selectionAlgorithmWindow");
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);
	// vtkNew<vtkInteractorStyleTrackballCamera> style;
	// renderWindowInteractor->SetInteractorStyle(style);
	renderer->SetBackground(colors->GetColor3d("navy").GetData());

	std::string filePath = "./geometrySample/Ref_XYZ.stp";
	if (fileExists(filePath)) {
		vtkLogF(INFO, "Geometry file is valid: \"%s\"", filePath.c_str());
	} else {
		vtkLogF(ERROR, "Invalid geometry file");
		return EXIT_FAILURE;
	}

	const char* myCString = filePath.c_str();
	Standard_CString CStringfilePath(myCString);
	TopoDS_Shape shape = readSTEP(CStringfilePath);

	if (!shape.IsNull()) {
		std::cout << "Shape loaded successfully!" << std::endl;
	} else {
		std::cerr << "Failed to load shape!" << std::endl;
		return EXIT_FAILURE;
	}
	vtkNew<vtkActor> actor = createVTKActor(shape);

	// Initialize Picker
	vtkSmartPointer<IVtkTools_ShapePicker> aShapePicker = vtkSmartPointer<IVtkTools_ShapePicker>::New();
	aShapePicker->SetTolerance(0.025);
	aShapePicker->SetRenderer(renderer);
	aShapePicker->SetSelectionMode(SM_Edge);

	vtkSmartPointer<InteractorStylePick> aStylePick = vtkSmartPointer<InteractorStylePick>::New();
	aStylePick->SetRenderer(renderer);
	aStylePick->SetPicker(aShapePicker);
	renderWindowInteractor->SetInteractorStyle(aStylePick);

	renderer->AddActor(actor);
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}