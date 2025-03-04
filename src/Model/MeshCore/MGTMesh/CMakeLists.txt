ADD_LIBRARY(MGTMesh
    MGTMesh_Algorithm.cpp
    MGTMesh_Scheme.cpp
    MGTMesh_MeshObject.cpp
    MGTMesh_Generator.cpp
    MGTMesh_ProxyMesh.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(MGTMesh PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        MGTMeshUtils
        NetgenPlugin
    )
else()
    TARGET_LINK_LIBRARIES(MGTMesh PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        MGTMeshUtils
        NetgenPlugin
    )
endif()

TARGET_INCLUDE_DIRECTORIES(MGTMesh PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMeshUtils
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/NetgenPlugin
)
