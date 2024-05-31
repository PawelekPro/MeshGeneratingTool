cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(meshGenerator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Define an option to include/exclude examples
option(BUILD_EXAMPLES "Build examples" ON)

# Set CMake to build in Debug mode
set(CMAKE_BUILD_TYPE Debug)

set(FORMS_PATH ${CMAKE_SOURCE_DIR}/src/UserInterface/Forms)
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${FORMS_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Project vars
######################################################################
MESSAGE("PROJECT NAME: ${PROJECT_NAME}")

SET ( PRJ_SOURCE_DIR ${PROJECT_SOURCE_DIR} )
SET ( PRJ_BINARY_DIR ${PROJECT_BINARY_DIR} )


# Qt
######################################################################
find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
    Widgets 
    OpenGLWidgets 
    QuickWidgets
    Core
    Gui
    Xml
    Core5Compat
    Qml
    Svg
)

MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "Qt FOUND" )
MESSAGE( STATUS "Qt VERSION: ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )


# VTK
######################################################################
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
    MESSAGE(FATAL_ERROR "vtk_example: Unable to find the VTK build folder.")
else()
    MESSAGE( STATUS "VTK FOUND" )
    MESSAGE( STATUS "VTK VERSION: ${VTK_VERSION}" )
    # MESSAGE( STATUS "VTK_LIBRARIES: ${VTK_LIBRARIES}" )
    MESSAGE( STATUS "VTK_USE_FILE: ${VTK_USE_FILE}" )
    MESSAGE( STATUS "-------------------------------------------------------------------------------" )
endif()

if(NOT(TARGET VTK::GUISupportQt))
    MESSAGE(FATAL_ERROR "VTK not built with Qt support.")
endif()


# OPENCASCADE
######################################################################
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

if (NOT OCC_FOUND AND NOT OpenCASCADE_FOUND)
    MESSAGE(FATAL_ERROR "OPENCASCADE NOT FOUND.")
else()
    MESSAGE( STATUS "${OCC_TYPE} VERSION: ${OCC_VERSION_STRING}" )
    MESSAGE( STATUS "${OCC_TYPE} include directory: ${OCC_INCLUDE_DIR}")
    MESSAGE( STATUS "${OCC_TYPE} shared libraries directory: ${OCC_LIBRARY_DIR}")
    MESSAGE( STATUS "-------------------------------------------------------------------------------" )
endif()


# GMSH
######################################################################
if(WIN32)
    find_path(GMSH_INCLUDE_DIR gmsh.h PATHS ${GMSH_ROOT}/include)
    find_path(GMSH_LIB_DIR gmsh.lib PATHS ${GMSH_ROOT}/lib)
else()
    FIND_PATH(GMSH_INCLUDE_DIR gmsh.h PATHS ${GMSH_ROOT}/include)
    SET(GMSH_LIB_DIR ${GMSH_ROOT}/lib)
    FIND_LIBRARY(GMSH_LIB libgmsh.so PATHS ${GMSH_LIB_DIR})
    FIND_PATH(GMSH_INC gmsh.h_cwrap PATHS ${GMSH_INCLUDE_DIR})
endif()


if(NOT GMSH_LIB)
    MESSAGE(FATAL_ERROR "Could not find libgmsh")
else()
    # Read the content of gmsh.h into a variable
    file(READ "${GMSH_INCLUDE_DIR}/gmsh.h" GMSH_H_CONTENT)
    # Extract version information using regular expressions
    # Major version
    file(STRINGS ${GMSH_INCLUDE_DIR}/gmsh.h GMSH_API_VERSION_MAJOR
        REGEX "#define GMSH_API_VERSION_MAJOR.*"
    )
    string(REGEX MATCH "[0-9]+" GMSH_API_VERSION_MAJOR ${GMSH_API_VERSION_MAJOR})
    # Minor version
    file(STRINGS ${GMSH_INCLUDE_DIR}/gmsh.h GMSH_API_VERSION_MINOR
        REGEX "#define GMSH_API_VERSION_MINOR.*"
    )
    string(REGEX MATCH "[0-9]+" GMSH_API_VERSION_MINOR ${GMSH_API_VERSION_MINOR})
    # Patch version
    file(STRINGS ${GMSH_INCLUDE_DIR}/gmsh.h GMSH_API_VERSION_PATCH
        REGEX "#define GMSH_API_VERSION_PATCH.*"
    )
    string(REGEX MATCH "[0-9]+" GMSH_API_VERSION_PATCH ${GMSH_API_VERSION_PATCH})

    # Print version information
    MESSAGE(STATUS "GMSH FOUND")
    MESSAGE(STATUS "GMSH VERSION: ${GMSH_API_VERSION_MAJOR}.${GMSH_API_VERSION_MINOR}.${GMSH_API_VERSION_PATCH}")
    MESSAGE(STATUS "GMSH include directory: ${GMSH_INCLUDE_DIR}")
    MESSAGE(STATUS "GMSH lib directory: ${GMSH_LIB_DIR}")
    MESSAGE(STATUS "-------------------------------------------------------------------------------")
endif()

if(NOT GMSH_INC)
    MESSAGE(FATAL_ERROR "Could not find gmsh.h_cwrap")
endif()

INCLUDE_DIRECTORIES(${GMSH_INC})


# rapidjson library for parsing json files
######################################################################
set(RAPIDJSON_PATH ${CMAKE_SOURCE_DIR}/src/ThirdParty/rapidjson)
file(STRINGS ${RAPIDJSON_PATH}/rapidjson.h RAPIDJSON_MAJOR_VERSION
    REGEX "#define RAPIDJSON_MAJOR_VERSION.*"
  )
  string(REGEX MATCH "[0-9]+" RAPIDJSON_MAJOR_VERSION ${RAPIDJSON_MAJOR_VERSION})

file(STRINGS ${RAPIDJSON_PATH}/rapidjson.h RAPIDJSON_MINOR_VERSION
    REGEX "#define RAPIDJSON_MINOR_VERSION.*"
)
string(REGEX MATCH "[0-9]+" RAPIDJSON_MINOR_VERSION ${RAPIDJSON_MINOR_VERSION})


MESSAGE( STATUS "RapidJSON FOUND" )
MESSAGE( STATUS "RapidJSON VERSION: ${RAPIDJSON_MAJOR_VERSION}.${RAPIDJSON_MINOR_VERSION}")
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
INCLUDE_DIRECTORIES(${RAPIDJSON_PATH})


# SARibbonBar library (ribbon bar widget)
# https://github.com/czyt1988/SARibbon?tab=readme-ov-file
######################################################################
set(SARIBBON_PATH ${CMAKE_SOURCE_DIR}/src/ThirdParty/SARibbon)
file(STRINGS ${SARIBBON_PATH}/SARibbon.h SA_RIBBON_BAR_VERSION_MAJ
    REGEX "#define SA_RIBBON_BAR_VERSION_MAJ.*"
  )
  string(REGEX MATCH "[0-9]+" SA_RIBBON_BAR_VERSION_MAJ ${SA_RIBBON_BAR_VERSION_MAJ})

file(STRINGS ${SARIBBON_PATH}/SARibbon.h SA_RIBBON_BAR_VERSION_MIN
    REGEX "#define SA_RIBBON_BAR_VERSION_MIN.*"
)
string(REGEX MATCH "[0-9]+" SA_RIBBON_BAR_VERSION_MIN ${SA_RIBBON_BAR_VERSION_MIN})

file(STRINGS ${SARIBBON_PATH}/SARibbon.h SA_RIBBON_BAR_VERSION_PAT
    REGEX "#define SA_RIBBON_BAR_VERSION_PAT.*"
)
string(REGEX MATCH "[0-9]+" SA_RIBBON_BAR_VERSION_PAT ${SA_RIBBON_BAR_VERSION_PAT})

MESSAGE( STATUS "SARibbonBar Library FOUND" )
MESSAGE( STATUS "SARibbonBar VERSION: ${SA_RIBBON_BAR_VERSION_MAJ}.${SA_RIBBON_BAR_VERSION_MIN}.${SA_RIBBON_BAR_VERSION_PAT}")
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
INCLUDE_DIRECTORIES(${SARIBBON_PATH})

# Advanced stylesheets (thirdparty)
# https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets?tab=readme-ov-file
set(STYLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/QtAdvancedStyleSheets/styles")
add_subdirectory(src/ThirdParty/QtAdvancedStyleSheets)

# Include the src directory
add_subdirectory(src)

if(BUILD_EXAMPLES)
    # Include the examples directory
    add_subdirectory(examples)
endif()

# Print info
######################################################################
STRING ( TOUPPER "${CMAKE_BUILD_TYPE}" MY_CMAKE_BUILD_TYPE )

MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CMAKE version       = ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" )
MESSAGE( STATUS "CMAKE_SYSTEM        = ${CMAKE_SYSTEM}" )
MESSAGE( STATUS "CMAKE_GENERATOR     = ${CMAKE_GENERATOR}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE    = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CMAKE_C_COMPILER    = ${CMAKE_C_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER  = ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "  FLAGS:" )
MESSAGE( STATUS )
MESSAGE( STATUS "CMAKE_C_FLAGS_${MY_CMAKE_BUILD_TYPE}          = ${CMAKE_C_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_${MY_CMAKE_BUILD_TYPE}        = ${CMAKE_CXX_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS_${MY_CMAKE_BUILD_TYPE} = ${CMAKE_EXE_LINKER_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "  MODULES:" )
MESSAGE( STATUS )

MESSAGE( STATUS "PRJ_SOURCE_DIR     = ${PRJ_SOURCE_DIR}" )
MESSAGE( STATUS "PRJ_BINARY_DIR	    = ${PRJ_BINARY_DIR}" )

MESSAGE( STATUS "-------------------------------------------------------------------------------" )


ADD_EXECUTABLE(meshGenerator
    ${GeometryCoreFiles} 
    ${UserInterfaceFiles}
    ${KernelFiles}
    ${RESOURCES_PATH}
)

TARGET_LINK_LIBRARIES (meshGenerator PRIVATE
    GeometryCore
    UserInterface
    Kernel
) 