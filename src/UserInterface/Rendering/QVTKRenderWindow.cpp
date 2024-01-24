#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLookupTable.h>

#include "QVTKRenderWindow.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <vtkPlatonicSolidSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

namespace
{
/** Get a specialised lookup table for the platonic solids.
 *
 * @return The lookup table.
 */
vtkNew<vtkLookupTable> GetPlatonicLUT();
} // namespace

namespace Rendering
{

QVTKRenderWindow::QVTKRenderWindow(QVTKOpenGLNativeWidget *vtkWidget)
    : _vtkWidget(vtkWidget)
{

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    _vtkWidget->setRenderWindow(renderWindow);

    vtkNew<vtkNamedColors> colors;

    auto lut = GetPlatonicLUT();

    vtkNew<vtkPlatonicSolidSource> source;
    source->SetSolidTypeToIcosahedron();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(source->GetOutputPort());
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0, 19);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->GetActiveCamera()->Azimuth(180.0);
    renderer->ResetCamera();
    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

    this->_vtkWidget->renderWindow()->AddRenderer(renderer);
    this->_vtkWidget->renderWindow()->SetWindowName(
        "RenderWindowUISingleInheritance");

    // Add the actors to the scene.
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
}
} // namespace Rendering

namespace
{

vtkNew<vtkLookupTable> GetPlatonicLUT()
{
    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(20);
    lut->SetTableRange(0.0, 19.0);
    lut->Build();
    lut->SetTableValue(0, 0.1, 0.1, 0.1);
    lut->SetTableValue(1, 0, 0, 1);
    lut->SetTableValue(2, 0, 1, 0);
    lut->SetTableValue(3, 0, 1, 1);
    lut->SetTableValue(4, 1, 0, 0);
    lut->SetTableValue(5, 1, 0, 1);
    lut->SetTableValue(6, 1, 1, 0);
    lut->SetTableValue(7, 0.9, 0.7, 0.9);
    lut->SetTableValue(8, 0.5, 0.5, 0.5);
    lut->SetTableValue(9, 0.0, 0.0, 0.7);
    lut->SetTableValue(10, 0.5, 0.7, 0.5);
    lut->SetTableValue(11, 0, 0.7, 0.7);
    lut->SetTableValue(12, 0.7, 0, 0);
    lut->SetTableValue(13, 0.7, 0, 0.7);
    lut->SetTableValue(14, 0.7, 0.7, 0);
    lut->SetTableValue(15, 0, 0, 0.4);
    lut->SetTableValue(16, 0, 0.4, 0);
    lut->SetTableValue(17, 0, 0.4, 0.4);
    lut->SetTableValue(18, 0.4, 0, 0);
    lut->SetTableValue(19, 0.4, 0, 0.4);
    return lut;
}

} // namespace