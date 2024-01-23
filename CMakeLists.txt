cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(meshGenerator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)

# Check compiler standert supports
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    add_definitions(-DCOMPILEDWITHC17)
    message(STATUS "Using flag -std=c++17.")
else()
    message(FATAL_ERROR
        "The compiler ${CMAKE_CXX_COMPILER} has no 
        required standard support. Please use a different C++ compiler.")
endif()


SET(CMAKE_PREFIX_PATH "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6")
SET(Qt6Widgets_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6Widgets")
SET(Qt6CoreTools_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6CoreTools")
SET(Qt6GuiTools_DIR "/opt/Qt/6.6.1/gcc_64/lib/cmake/Qt6GuiTools")

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
    Widgets 
    OpenGLWidgets 
    Gui
)

message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")

set(UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface")
set(FORMS_PATH "${UI_FILES}/Forms")
set(CAD_MANAGER "${CMAKE_CURRENT_SOURCE_DIR}/src/CADManager")


set(CMAKE_AUTOUIC_SEARCH_PATHS ${FORMS_PATH})
set(PROJECT_SOURCES
    ${UI_FILES}/main.cpp
    ${UI_FILES}/mainwindow.cpp
    ${UI_FILES}/mainwindow.h
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Try to find OCC
set(OCC_INCLUDE_DIR "/opt/opencascade/include/opencascade")
find_package(OCC REQUIRED)

if(OCC_FOUND) 
    include_directories(SYSTEM ${OCC_INCLUDE_DIR})
    link_directories(${OCC_LIBRARY_DIR})
endif()

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    CommonTransforms
    FiltersSources
    InteractionStyle
    FiltersSources
    RenderingAnnotation
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    GUISupportQt
    RenderingQt
)

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
include_directories("./src")
include_directories("./include")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(meshGenerator
        MANUAL_FINALIZATION
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
    ${VTK_LIBRARIES}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(meshGenerator)
endif()