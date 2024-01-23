#include <iostream>
#include <filesystem>

#include <vtkActor.h>
#include <vtkBoxWidget.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkLogger.h>

#include "CADFileManager.h"

using namespace std::filesystem;


CAD::CADFileManager* CAD::CADFileManager::m_instance(nullptr);
std::mutex CAD::CADFileManager::m_mutex;

CAD::CADFileManager*
CAD::CADFileManager::instance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_instance == nullptr) {
        m_instance = new CADFileManager{};
        // Initializing vtk logger
        vtkLogger::EnableUnsafeSignalHandler = false;
        vtkLogger::Init();
    }
    std::cout << "Returning instance of CADFileManager" << std::endl;
    return m_instance;
}


std::string
CAD::CADFileManager::load(const std::string& fileName)
{
    if (!std::filesystem::exists(fileName)) {
        auto message = "File " + fileName + " can not be found.";
        vtkLogF(ERROR, message.c_str());
        auto errorCode = std::make_error_code(std::errc::no_such_file_or_directory);
        throw std::filesystem::filesystem_error(message, errorCode);
    }
    auto baseName = std::filesystem::path{fileName}.stem().generic_string();
    vtkLogF(INFO, "Geometry file path: \"%s\"", fileName.c_str());

    return baseName;
}

std::string
CAD::CADFileManager::draw(vtkRenderer* renderer)
{
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(100);

    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(colors->GetColor3d("Bisque").GetData());
    
    renderer->AddActor(coneActor);
    vtkLogF(INFO, "Rendering object");

    void* address = reinterpret_cast<void*>(coneActor.GetPointer());
    std::string addressString = std::to_string(reinterpret_cast<uintptr_t>(address));

    return addressString;

}