ADD_LIBRARY(GMSHPlugin
    GMSHPlugin_Mesher.cpp
    GMSHPlugin_Parameters.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GMSHPlugin PUBLIC
        spdlog::spdlog_header_only
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        ${GMSH_LIB}
        MGTMesh
        MGTMeshUtils)
else()
    TARGET_LINK_LIBRARIES(GMSHPlugin PUBLIC
        spdlog::spdlog_header_only    
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        ${GMSH_LIB}
        MGTMesh
        MGTMeshUtils)
endif()

include_directories(${GMSH_INCLUDE_DIR})

TARGET_INCLUDE_DIRECTORIES(GMSHPlugin PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/GMSHPlugin
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMesh
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMeshUtils
)
