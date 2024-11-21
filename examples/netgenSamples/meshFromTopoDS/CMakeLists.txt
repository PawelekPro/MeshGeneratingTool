cmake_minimum_required(VERSION 3.15)

project(meshFromTopoDS)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES "main.cpp")

include_directories(${NETGEN_INCLUDE_DIRS})

add_executable(meshFromTopoDS ${PROJECT_SOURCES})

target_link_libraries(meshFromTopoDS PRIVATE
    nglib 
    ngcore
    TKernel       
    TKG3d
    TKG2d
    TKGeomBase
    TKMath
    TKTopAlgo
    TKBO
    TKSTEP
    TKSTEPAttr
    TKSTEP209
    TKSTEPBase
    TKXSBase
    TKBRep   
    TKPrim     
    TKFillet         
    TKShHealing     
    TKGeomAlgo   
    TKMesh
    ${VTK_LIBRARIES}     
)

vtk_module_autoinit(
  TARGETS meshFromTopoDS
  MODULES ${VTK_LIBRARIES}
)