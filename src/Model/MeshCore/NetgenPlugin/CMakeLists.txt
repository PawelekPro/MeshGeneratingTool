ADD_LIBRARY(NetgenPlugin
    NetgenPlugin_NetgenLibWrapper.cpp
    NetgenPlugin_Netgen2VTK.cpp
    NetgenPlugin_Parameters.cpp
    NetgenPlugin_MeshInfo.cpp
    NetgenPlugin_Mesher.cpp
)


TARGET_LINK_LIBRARIES(NetgenPlugin PUBLIC
    spdlog::spdlog_header_only    
    ${OCC_LIBRARIES}
    ${VTK_LIBRARIES}
    ${NETGEN_LIBRARIES}
    MGTMesh
    MGTMeshUtils
)

include_directories(${NETGEN_INCLUDE_DIRS})
target_compile_definitions(NetgenPlugin PUBLIC NETGENPLUGIN_EXPORTS NETGEN_V6)

TARGET_INCLUDE_DIRECTORIES(NetgenPlugin PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/NetgenPlugin
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMesh
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMeshUtils
)
