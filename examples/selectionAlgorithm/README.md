## OpenCascade VIS selection algorithm (IVtkTools_ShapePicker) usage example

An example of using an algorithm from the OpenCascade VIS (VTK integration service) library. The example is based on the use of IVtkTools_ShapePicker. STEPControl_Reader was used to load STEP. The STEP file is loaded as a single TopoDS_Shape, on which selection operations are then performed using IVtkTools_SubPolyDataFilter. The geometrySample directory contains sample STEP geometries.

The current selection method is set using the function:
```
_picker->SetSelectionMode(SM_Face);
```
where the function argument is the IVtk_SelectionMode enum (see [IVtk_Types.hxx File Reference](https://dev.opencascade.org/doc/occt-7.6.0/refman/html/_i_vtk___types_8hxx.html#acc7068231bfa08df9dc6c6ac9e26df02)).

The implementation includes the introduction of a custom pipeline IVtk_CustomSelectionPipeline, which is responsible for handling events like the selection of a specific entity. Events are handled by custom class IVtk_CustmInteractorStyle, which inherits from vtkInteractorStyleTrackballCamera and overrides corresponding methods of the base class.

## Build Instructions

The example is compiled together with the source code if the ```BUILD_EXAMPLES``` option is set to ON.