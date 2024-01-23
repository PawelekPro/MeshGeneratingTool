#include <vtkNamedColors.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include "CADFileManager.h"


int main() {
    auto cadManager = CAD::CADFileManager::instance();
    std::string path = "/home/pgilewicz/OcctProject/step_occt/example.cpp";
    cadManager->load(path);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto backGround = std::array<unsigned char, 4>{{26, 51, 102, 255}};
    colors->SetColor("BkgColor", backGround.data());

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    // Draw actor
    cadManager->draw(renderer);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Zoom(1.5);

    auto render = vtkSmartPointer<vtkRenderWindow>::New();
    render->SetSize(600, 600);
    render->AddRenderer(renderer);
    std::string windowName = "RenderingTesting";
    render->SetWindowName(windowName.c_str());

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    auto style = vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();
    interactor->SetRenderWindow(render);
    interactor->SetInteractorStyle(style);

    auto axes = vtkSmartPointer<vtkAxesActor>::New();
    auto widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(interactor);
    widget->SetViewport(0.0, 0.0, 0.4, 0.4);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    renderer->ResetCamera();

    // This starts the event loop and as a side effect causes an initial render.
    render->Render();
    interactor->Start();

    return EXIT_SUCCESS;
}