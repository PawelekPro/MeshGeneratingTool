cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(meshGenerator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)

# Check compiler standert supports
# include(CheckCXXCompilerFlag)
# CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

# if(COMPILER_SUPPORTS_CXX17)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#     add_definitions(-DCOMPILEDWITHC17)
#     message(STATUS "Using flag -std=c++17.")
# else()
#     message(FATAL_ERROR
#         "The compiler ${CMAKE_CXX_COMPILER} has no 
#         required standard support. Please use a different C++ compiler.")
# endif()


SET(CMAKE_PREFIX_PATH "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6")
SET(Qt6Widgets_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6Widgets")
SET(Qt6CoreTools_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6CoreTools")
SET(Qt6GuiTools_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6GuiTools")

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
    Widgets 
    OpenGLWidgets 
    Core
    Gui
)

message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")

set(KERNEL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/Kernel")
set(UI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface")
set(UI_UTILS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface/GraphicalUtils")
set(FORMS_PATH "${UI_PATH}/Forms")
set(GEOM_CORE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/GeometryCore")
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
#    ${UI_PATH}/Rendering/QVTKNavigationWidget.h
#    ${UI_PATH}/Rendering/QVTKNavigationWidget.cpp
#    ${UI_PATH}/Rendering/QVTKToolBar.h
#    ${UI_PATH}/Rendering/QVTKToolBar.cpp
    ${GEOM_CORE_PATH}/Geometry/GeometryFunctions.cpp
    ${GEOM_CORE_PATH}/Geometry/GeometryFunctions.h
    ${GEOM_CORE_PATH}/STEPPlugin/STEPPlugin.h
    ${GEOM_CORE_PATH}/STEPPlugin/STEPPlugin.cpp
    ${GEOM_CORE_PATH}/STEPPlugin/STEPPluginOperations.cpp
    ${GEOM_CORE_PATH}/STEPPlugin/STEPPluginOperations.h
    ${GEOM_CORE_PATH}/STLPlugin/STLPluginOperations.cpp
    ${GEOM_CORE_PATH}/STLPlugin/STLPluginOperations.h
)

include_directories(
    ${KERNEL_PATH}
    ${UI_PATH}
    ${UI_UTILS_PATH}
    ${GEOM_CORE_PATH}
    ${GEOM_CORE_PATH}/GUI
    ${GEOM_CORE_PATH}/Geometry
    ${GEOM_CORE_PATH}/STEPPlugin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Try to find OCC
set(OCC_INCLUDE_DIR "/opt/opencascade/include/opencascade")
find_package(OCC REQUIRED)

if(OCC_FOUND) 
    include_directories(SYSTEM ${OCC_INCLUDE_DIR})
    link_directories(${OCC_LIBRARY_DIR})
endif()

if(WIN32)
    find_package(VTK QUIET REQUIRED vtkCommonCore vtkCommonDataModel
    vtkCommonColor vtkFiltersCore vtkFiltersSources vtkInteractionStyle
    vtkRenderingContextOpenGL2 vtkRenderingCore vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2 vtkRenderingOpenGL2 vtkIOGeometry
    vtkInfovisLayout vtkViewsInfovis vtkGUISupportQt vtkRenderingQt)
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

file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")
SET(INCLUDE_DIRS "")
foreach(_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)
# include_directories("./include")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(meshGenerator
        MANUAL_FINALIZATION
        ${RESOURCES_PATH}
        ${PROJECT_SOURCES}
    )
else()
    add_executable(meshGenerator
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(meshGenerator PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Core
    ${VTK_LIBRARIES}
    ${OCC_LIBRARIES}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(meshGenerator)
endif()