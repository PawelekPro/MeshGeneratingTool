cmake_minimum_required(VERSION 3.15)

project(surfMeshFromSTEP)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES "main.cpp")

include_directories(${NETGEN_INCLUDE_DIRS})

add_executable(surfMeshFromSTEP ${PROJECT_SOURCES})

target_link_libraries(surfMeshFromSTEP PRIVATE
    nglib 
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
    TKFillet         
    TKShHealing     
    TKGeomAlgo   
    ${VTK_LIBRARIES}    
)

vtk_module_autoinit(
  TARGETS surfMeshFromSTEP
  MODULES ${VTK_LIBRARIES}
)