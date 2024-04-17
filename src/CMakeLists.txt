# Qt
######################################################################
find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS 
    Widgets 
    OpenGLWidgets 
    Core
    Gui
    Xml
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
    find_library(GMSH_LIB gmsh)
    find_path(GMSH_INC gmsh.h_cwrap)

else()
    FIND_PATH(GMSH_INCLUDE_DIR gmsh.h PATHS ${GMSH_ROOT}/include)
    SET(GMSH_LIB_DIR ${GMSH_ROOT}/lib)
    FIND_LIBRARY(GMSH_LIB libgmsh.so PATHS ${GMSH_LIB_DIR})
    FIND_PATH(GMSH_INC gmsh.h_cwrap PATHS ${GMSH_INCLUDE_DIR})
endif()


if(NOT GMSH_LIB)
    MESSAGE(FATAL_ERROR "Could not find libgmsh")
else()
    set(GMSH_INC ${GMSH_ROOT}/include)
    # Read the content of gmsh.h into a variable
    file(READ "${GMSH_INC}/gmsh.h" GMSH_H_CONTENT)
    # Extract version information using regular expressions
    # Major version
    file(STRINGS ${GMSH_INC}/gmsh.h GMSH_API_VERSION_MAJOR
        REGEX "#define GMSH_API_VERSION_MAJOR.*"
    )
    string(REGEX MATCH "[0-9]+" GMSH_API_VERSION_MAJOR ${GMSH_API_VERSION_MAJOR})
    # Minor version
    file(STRINGS ${GMSH_INC}/gmsh.h GMSH_API_VERSION_MINOR
        REGEX "#define GMSH_API_VERSION_MINOR.*"
    )
    string(REGEX MATCH "[0-9]+" GMSH_API_VERSION_MINOR ${GMSH_API_VERSION_MINOR})
    # Patch version
    file(STRINGS ${GMSH_INC}/gmsh.h GMSH_API_VERSION_PATCH
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


# Modules
######################################################################
ADD_SUBDIRECTORY(GeometryCore) 
ADD_SUBDIRECTORY(UserInterface)
ADD_SUBDIRECTORY(Kernel)


