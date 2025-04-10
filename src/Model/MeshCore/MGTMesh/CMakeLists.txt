ADD_LIBRARY(MGTMesh
        MGTMesh_Algorithm.cpp
        MGTMesh_Scheme.cpp
        MGTMesh_MeshObject.cpp
        MGTMesh_Generator.cpp
        MGTMesh_ProxyMesh.cpp
        MGTMesh_MeshParameters.cpp
)


TARGET_LINK_LIBRARIES(MGTMesh PUBLIC
    ${OCC_LIBRARIES}
    ${VTK_LIBRARIES}
    MGTMeshUtils
    NetgenPlugin
)


TARGET_INCLUDE_DIRECTORIES(MGTMesh PUBLIC
        ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMeshUtils
        ${PRJ_SOURCE_DIR}/src/Model/MeshCore/NetgenPlugin
)
