add_subdirectory(MGTMeshUtils)
add_subdirectory(NetgenPlugin)
add_subdirectory(MGTMesh)

ADD_LIBRARY(MeshCore)

TARGET_LINK_LIBRARIES(MeshCore PUBLIC
    MGTMesh
    MGTMeshUtils
    NetgenPlugin
)
