#ifndef IVtk_CustomInteractorStyle_H
#define IVtk_CustomInteractorStyle_H

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

// VIS includes
#include <IVtkDraw_HighlightAndSelectionPipeline.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_ShapeObject.hxx>
#include <IVtkTools_ShapePicker.hxx>
#include <IVtkTools_SubPolyDataFilter.hxx>

#include "IVtk_CustomSelectionPipeline.hxx"

class IVtk_CustomInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	static IVtk_CustomInteractorStyle* New();
	vtkTypeMacro(IVtk_CustomInteractorStyle, vtkInteractorStyleTrackballCamera);

public:
	void SetRenderer(const vtkSmartPointer<vtkRenderer>& theRenderer);
	vtkSmartPointer<vtkRenderer> GetRenderer() const;
	void SetPicker(const vtkSmartPointer<IVtkTools_ShapePicker>& thePicker);
	vtkSmartPointer<IVtkTools_ShapePicker> GetPicker() const;
	void setPipeline(const Handle(IVtk_CustomSelectionPipeline) pipeline) {
		_pipeline = pipeline;
	}

	// Overriding:
public:
	virtual void OnRightButtonDown() {};
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();

private:
	IVtk_CustomInteractorStyle(const IVtk_CustomInteractorStyle&);
	void operator=(const IVtk_CustomInteractorStyle&);

private:
	IVtk_CustomInteractorStyle();
	~IVtk_CustomInteractorStyle() { }

private:
	vtkSmartPointer<vtkRenderer> _renderer;
	vtkSmartPointer<IVtkTools_ShapePicker> _picker;
	Handle(IVtk_CustomSelectionPipeline) _pipeline;
};

#endif