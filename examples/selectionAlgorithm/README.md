## OpenCascade VIS selection algorithm (IVtkTools_ShapePicker) usage example

An example of using an algorithm from the OpenCascade VIS (VTK integration service) library. The example is based on the use of IVtkTools_ShapePicker. STEPControl_Reader was used to load STEP. The STEP file is loaded as a single TopoDS_Shape, on which selection operations are then performed using IVtkTools_SubPolyDataFilter. The geometrySample directory contains sample STEP geometries.

The current selection method is set using the function:
```
m_picker->SetSelectionMode(SM_Face);
```
where the function argument is the IVtk_SelectionMode enum (see [IVtk_Types.hxx File Reference](https://dev.opencascade.org/doc/occt-7.6.0/refman/html/_i_vtk___types_8hxx.html#acc7068231bfa08df9dc6c6ac9e26df02))
## Build Instructions
```bash
mkdir build && cd build
```

```bash
cmake ..
```

```bash
make
```