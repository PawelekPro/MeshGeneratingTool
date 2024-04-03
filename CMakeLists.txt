cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(meshGenerator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)


# add gmsh
if(WIN32)
    set(GMSH_ROOT "D:/GmshSDK/gmsh-4.12.2")
else()
    set(GMSH_ROOT "your path to gmsh sdk ...")
endif()

find_path(GMSH_INCLUDE_DIR gmsh.h PATHS ${GMSH_ROOT}/include)
find_path(GMSH_LIB_DIR gmsh.lib PATHS ${GMSH_ROOT}/lib)
find_library(GMSH_LIB gmsh)
find_path(GMSH_INC gmsh.h_cwrap)

if(NOT GMSH_LIB)
  message(FATAL_ERROR "Could not find libgmsh")
endif()
if(NOT GMSH_INC)
  message(FATAL_ERROR "Could not find gmsh.h_cwrap")
endif()


find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
    Widgets 
    OpenGLWidgets 
    Core
    Gui
    Xml
)

message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")

set(KERNEL_PATH ${CMAKE_SOURCE_DIR}/src/Kernel)
set(UI_PATH ${CMAKE_SOURCE_DIR}/src/UserInterface)
set(UI_UTILS_PATH ${CMAKE_SOURCE_DIR}/src/UserInterface/GraphicalUtils)
set(FORMS_PATH ${UI_PATH}/Forms)
set(GEOM_CORE_PATH ${CMAKE_SOURCE_DIR}/src/GeometryCore)
set(MESH_CORE_PATH ${CMAKE_SOURCE_DIR}/src/MeshCore)
set(MODEL_PATH ${CMAKE_SOURCE_DIR}/src/Model)

set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")

set(CMAKE_AUTOUIC_SEARCH_PATHS ${FORMS_PATH})

set(PROJECT_SOURCES
    ${KERNEL_PATH}/AppQuickStart.cpp
    ${UI_PATH}/MainWindow.cpp
    ${UI_PATH}/MainWindow.h
    ${UI_UTILS_PATH}/ProgressBar.cpp
    ${UI_UTILS_PATH}/ProgressBar.h
    ${UI_UTILS_PATH}/TreeStructure.cpp
    ${UI_UTILS_PATH}/TreeStructure.h
    ${UI_UTILS_PATH}/Configuration.h
    ${UI_PATH}/Rendering/QVTKRenderWindow.cpp
    ${UI_PATH}/Rendering/QVTKRenderWindow.h
    ${UI_PATH}/Rendering/QVTKInteractorStyle.cpp
    ${UI_PATH}/Rendering/QVTKInteractorStyle.h
   ${UI_PATH}/Rendering/QVTKNavigationWidget.h
   ${UI_PATH}/Rendering/QVTKNavigationWidget.cpp
#    ${UI_PATH}/Rendering/QVTKToolBar.h
#    ${UI_PATH}/Rendering/QVTKToolBar.cpp
    ${GEOM_CORE_PATH}/Geometry/Geometry.h
    ${GEOM_CORE_PATH}/Geometry/Geometry.cpp
    ${GEOM_CORE_PATH}/GeometryImporter/GeometryImporter.h
    ${GEOM_CORE_PATH}/GeometryImporter/GeometryImporter.cpp
    ${GEOM_CORE_PATH}/GeometryImporter/STEPImporter.h
    ${GEOM_CORE_PATH}/GeometryImporter/STEPImporter.cpp
    ${GEOM_CORE_PATH}/GeometryImporter/STEPImporter.h
    ${GEOM_CORE_PATH}/GeometryImporter/STEPImporter.cpp
    ${GEOM_CORE_PATH}/GeometryImporter/STLImporter.h
    ${GEOM_CORE_PATH}/GeometryImporter/STLImporter.cpp
    
    ${MESH_CORE_PATH}/Mesh.h
    ${MESH_CORE_PATH}/Mesh.cpp
    ${MODEL_PATH}/Model.h
    ${MODEL_PATH}/Model.cpp
)

set(INCLUDE_DIRS
    ${KERNEL_PATH}

    ${UI_PATH}
    ${UI_PATH}/GraphicalUtils
    ${UI_PATH}/Rendering
    ${UI_PATH}/Forms

    ${GEOM_CORE_PATH}
    ${GEOM_CORE_PATH}/GUI
    ${GEOM_CORE_PATH}/Geometry
    ${GEOM_CORE_PATH}/GeometryImporter
    ${MESH_CORE_PATH}
    ${MODEL_PATH}

    ${GMSH_INC}
    )

include_directories(
    ${INCLUDE_DIRS}
)
    

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Try to find OCC
if(WIN32)
    find_package(OpenCASCADE REQUIRED)
    if(OpenCASCADE_INCLUDE_DIR) 
        include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR})
        link_directories(${OpenCASCADE_LIBRARY_DIR})
    endif()
else()
    find_package(OCC REQUIRED)
    set(OCC_INCLUDE_DIR "/opt/opencascade/include/opencascade")
    if(OCC_INCLUDE_DIR) 
        include_directories(SYSTEM ${OCC_INCLUDE_DIR})
        link_directories(${OCC_LIBRARY_DIR})
    endif()
endif()

if(WIN32)
    find_package(VTK QUIET REQUIRED CommonCore CommonDataModel
    CommonColor FiltersCore FiltersSources InteractionStyle
    RenderingContextOpenGL2 RenderingCore RenderingFreeType
    RenderingGL2PSOpenGL2 RenderingOpenGL2 IOGeometry
    InfovisLayout ViewsInfovis GUISupportQt RenderingQt RenderingAnnotation)
else()
    find_package(VTK QUIET REQUIRED vtkCommonCore vtkCommonDataModel
    vtkCommonColor vtkFiltersCore vtkFiltersSources vtkInteractionStyle
    vtkInteractionWidgets vtkRenderingAnnotation vtkRenderingContextOpenGL2
    vtkRenderingCore vtkRenderingFreeType vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2 vtkIOGeometry vtkInfovisLayout vtkViewsInfovis
    vtkFiltersParallelDIY2 vtkGUISupportQt vtkRenderingQt)
endif()


if (NOT VTK_FOUND)
  message(FATAL_ERROR "vtk_example: Unable to find the VTK build folder.")
else()
    message(STATUS "VTK_VERSION: ${VTK_VERSION}")
    message(STATUS "VTK_LIBRARIES: ${VTK_LIBRARIES}")
    message(STATUS "VTK_USE_FILE: ${VTK_USE_FILE}")
endif()

if(NOT(TARGET VTK::GUISupportQt))
  message(FATAL_ERROR "VTK not built with Qt support.")
endif()


add_executable(meshGenerator
    ${PROJECT_SOURCES}
    ${RESOURCES_PATH}
)


target_link_libraries(meshGenerator PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
    ${VTK_LIBRARIES}
    ${GMSH_LIB}
)

if(WIN32)
    target_link_libraries(meshGenerator PRIVATE 
        ${OpenCASCADE_LIBRARIES})
else()
    target_link_libraries(meshGenerator PRIVATE
        ${OCC_LIBRARIES})
endif()

