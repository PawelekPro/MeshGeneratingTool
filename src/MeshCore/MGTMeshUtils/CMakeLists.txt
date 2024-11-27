ADD_LIBRARY(MGTMeshUtils
    MGTMeshUtils_ControlPoint.cpp
    MGTMeshUtils_ViscousLayers.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(MGTMeshUtils PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        MGTMesh)
else()
    TARGET_LINK_LIBRARIES(MGTMeshUtils PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        MGTMesh)
endif()

include_directories(
    ${PRJ_SOURCE_DIR}/src/MeshCore/MGTMesh
)
