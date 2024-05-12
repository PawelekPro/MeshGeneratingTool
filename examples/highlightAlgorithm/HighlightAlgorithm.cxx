// Built-in
#include <filesystem>

// VTK includes
#include <vtkActor.h>
#include <vtkAxes.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkLogger.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkXOpenGLRenderWindow.h>

// VIS includes
#include <IVtkDraw_HighlightAndSelectionPipeline.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapeObject.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>

// OCCT includes
#include <BRepPrimAPI_MakeBox.hxx>
#include <Prs3d_Drawer.hxx>
#include <STEPControl_Reader.hxx>
#include <TColStd_MapIteratorOfPackedMapOfInteger.hxx>
#include <TColStd_PackedMapOfInteger.hxx>

#include "IVtk_CustomInteractorStyle.hxx"
#include "IVtk_CustomSelectionPipeline.hxx"

//-----------------------------------------------------------------------------
// Global context
//-----------------------------------------------------------------------------

namespace CTX {
TopoDS_Shape Shape;
Handle(ShapePipeline) ShapePL;
Handle(ShapePipeline) ShapeHiliPL;
vtkSmartPointer<vtkXOpenGLRenderWindow> RenderWindow;
};

//-----------------------------------------------------------------------------
// Interactor Style for picking
//-----------------------------------------------------------------------------

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

	// Overriding:
public:
	// virtual void OnRightButtonDown() {
	// 	vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	// 	// Invoke observers
	// 	this->InvokeEvent(EVENT_TRANSFORM, NULL);
	// }

	virtual void OnMouseMove() {
		m_picker->SetSelectionMode(SM_Face);

		Standard_Integer aPos[2] = { this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1] };
		m_picker->Pick(aPos[0], aPos[1], 0);

		vtkSmartPointer<vtkActorCollection> anActorCollection = m_picker->GetPickedActors();
		anActorCollection->InitTraversal();
		while (vtkActor* anActor = anActorCollection->GetNextActor()) {

			IVtkTools_ShapeDataSource* aDataSource = IVtkTools_ShapeObject::GetShapeSource(anActor);
			if (!aDataSource) {
				continue;
			}

			IVtkOCC_Shape::Handle anOccShape = aDataSource->GetShape();
			if (anOccShape.IsNull()) {
				continue;
			}
			IVtk_IdType aShapeID = anOccShape->GetId();
		}
		vtkInteractorStyleTrackballCamera::OnMouseMove();
	}

	virtual void OnLeftButtonDown() {
		// Set selection mode only here in order to have access to rendered actors
		m_picker->SetSelectionMode(SM_Face);

		// Invoke basic method
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

		Standard_Integer aPos[2] = { this->Interactor->GetEventPosition()[0],
			this->Interactor->GetEventPosition()[1] };
		m_picker->Pick(aPos[0], aPos[1], 0);

		// Traversing results
		vtkActor* aPickedActor = NULL;
		vtkSmartPointer<vtkActorCollection> anActorCollection = m_picker->GetPickedActors();
		//
		if (anActorCollection && anActorCollection->GetNumberOfItems() > 0) {
			anActorCollection->InitTraversal();
			while (vtkActor* anActor = anActorCollection->GetNextActor()) {
				aPickedActor = anActor;
				IVtkTools_ShapeDataSource* aDataSource = IVtkTools_ShapeObject::GetShapeSource(anActor);
				if (!aDataSource)
					continue;

				// Access initial shape wrapper
				IVtkOCC_Shape::Handle aShapeWrapper = aDataSource->GetShape();
				if (aShapeWrapper.IsNull())
					continue;

				IVtk_IdType aShapeID = aShapeWrapper->GetId();
				IVtk_ShapeIdList subShapeIds = m_picker->GetPickedSubShapesIds(aShapeID);

				// Get IDs of cells for picked sub-shapes.
				TColStd_PackedMapOfInteger aCellMask;
				for (IVtk_ShapeIdList::Iterator sIt(subShapeIds); sIt.More(); sIt.Next()) {
					aCellMask.Add((int)sIt.Value());
					const TopoDS_Shape& aSubShape = aShapeWrapper->GetSubShape(sIt.Value());
					cout << "--------------------------------------------------------------" << endl;
					cout << "Sub-shape ID: " << sIt.Value() << endl;
					cout << "Sub-shape type: " << aSubShape.TShape()->DynamicType()->Name() << endl;
				}

				CTX::ShapeHiliPL->InitSubPolyFilter(aCellMask);
				CTX::ShapeHiliPL->Update();
				break;
			}
		}
	}

private:
	InteractorStylePick(const InteractorStylePick&);
	void operator=(const InteractorStylePick&);

private:
	InteractorStylePick()
		: vtkInteractorStyleTrackballCamera() { }
	~InteractorStylePick() { }

private:
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<IVtkTools_ShapePicker> m_picker;
	Handle(ShapePipelineMap) myPipelines;
};

vtkStandardNewMacro(InteractorStylePick);

//-----------------------------------------------------------------------------
// Loading STEP file
//-----------------------------------------------------------------------------

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

bool fileExists(const std::string& filename) {
	return std::filesystem::exists(filename);
}

//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------

int main(int, char**) {
	// Initialize Renderer & Render Window
	vtkSmartPointer<vtkRenderer> aRenderer = vtkSmartPointer<vtkRenderer>::New();
	aRenderer->GetActiveCamera()->ParallelProjectionOn();
	aRenderer->LightFollowCameraOn();
	CTX::RenderWindow = vtkSmartPointer<vtkXOpenGLRenderWindow>::New();
	CTX::RenderWindow->AddRenderer(aRenderer);

	// Initialize Picker
	vtkSmartPointer<IVtkTools_ShapePicker> aShapePicker = vtkSmartPointer<IVtkTools_ShapePicker>::New();
	aShapePicker->SetTolerance(0.025);
	aShapePicker->SetRenderer(aRenderer);

	vtkNew<vtkNamedColors> colors;
	aRenderer->SetBackground(colors->GetColor3d("navy").GetData());
	std::string filePath = "./geometrySample/Ref_XYZ.stp";
	if (fileExists(filePath)) {
		vtkLogF(INFO, "Geometry file is valid: \"%s\"", filePath.c_str());
	} else {
		vtkLogF(ERROR, "Invalid geometry file");
		return EXIT_FAILURE;
	}
	const char* myCString = filePath.c_str();
	Standard_CString CStringfilePath(myCString);

	// Create test OCCT shape
	// TopoDS_Shape theShape = BRepPrimAPI_MakeBox(60, 80, 90).Shape();

	CTX::Shape = readSTEP(CStringfilePath);

	// // Create VTK pipeline enriched with VIS stuff for initial shape
	// CTX::ShapePL = new ShapePipeline();
	// CTX::ShapePL->Init(CTX::Shape);
	// CTX::ShapePL->AddToRenderer(aRenderer);

	// // Create VIS pipeline for highlighting
	// CTX::ShapeHiliPL = new ShapePipeline(true);
	// CTX::ShapeHiliPL->Init(CTX::Shape);
	// CTX::ShapeHiliPL->AddToRenderer(aRenderer);

	static Standard_Integer ShapeID = 0;
	++ShapeID;

	Handle(IVtk_CustomSelectionPipeline) pipeline
		= new IVtk_CustomSelectionPipeline(CTX::Shape, ShapeID);
	pipeline->AddToRenderer(aRenderer);

	// Initialize Interactor Style
	vtkSmartPointer<IVtk_CustomInteractorStyle>
		aStylePick = vtkSmartPointer<IVtk_CustomInteractorStyle>::New();
	aStylePick->SetRenderer(aRenderer);
	aStylePick->SetPicker(aShapePicker);
	aStylePick->setPipeline(pipeline);

	// Initialize Interactor
	vtkSmartPointer<vtkRenderWindowInteractor> aRenInter = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	aRenInter->SetRenderWindow(CTX::RenderWindow);
	aRenInter->SetInteractorStyle(aStylePick);
	aRenderer->ResetCamera();

	// Start rendering
	CTX::RenderWindow->Render();
	aRenInter->Start();

	return EXIT_SUCCESS;
}