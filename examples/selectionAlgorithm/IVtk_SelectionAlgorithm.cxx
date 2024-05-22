// Built-in
#include <filesystem>

// VTK includes
#include <vtkLogger.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

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
#include <STEPControl_Reader.hxx>

#include "IVtk_CustomInteractorStyle.hxx"
#include "IVtk_CustomSelectionPipeline.hxx"

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
	vtkSmartPointer<vtkRenderWindow> RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	RenderWindow->AddRenderer(aRenderer);

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
	// TopoDS_Shape Shape = BRepPrimAPI_MakeBox(60, 80, 90).Shape();

	TopoDS_Shape Shape = readSTEP(CStringfilePath);

	// Initialize Picker
	vtkSmartPointer<IVtkTools_ShapePicker> aShapePicker = vtkSmartPointer<IVtkTools_ShapePicker>::New();

	aShapePicker->SetRenderer(aRenderer);
	static Standard_Integer ShapeID = 0;
	++ShapeID;
	Handle(IVtk_CustomSelectionPipeline) pipeline
		= new IVtk_CustomSelectionPipeline(Shape, ShapeID);
	pipeline->AddToRenderer(aRenderer);

	// Initialize Interactor Style
	vtkSmartPointer<IVtk_CustomInteractorStyle>
		aStylePick = vtkSmartPointer<IVtk_CustomInteractorStyle>::New();
	aStylePick->SetRenderer(aRenderer);
	aStylePick->SetPicker(aShapePicker);
	aStylePick->setPipeline(pipeline);

	// Initialize Interactor
	vtkSmartPointer<vtkRenderWindowInteractor> aRenInter = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	aRenInter->SetRenderWindow(RenderWindow);
	aRenInter->SetInteractorStyle(aStylePick);
	aRenderer->ResetCamera();

	// Start rendering
	RenderWindow->Render();
	aRenInter->Start();

	return EXIT_SUCCESS;
}