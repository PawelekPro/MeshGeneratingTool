ADD_LIBRARY(NetgenPlugin SHARED
    NetgenPlugin_NetgenLibWrapper.cpp
    NetgenPlugin_Netgen2VTK.cpp
    NetgenPlugin_Parameters.cpp
    NetgenPlugin_MeshInfo.cpp
    NetgenPlugin_Mesher.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(NetgenPlugin PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        ${NETGEN_LIBRARIES}
        MGTMesh
        MGTMeshUtils)
else()
    TARGET_LINK_LIBRARIES(NetgenPlugin PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        ${NETGEN_LIBRARIES}
        MGTMesh
        MGTMeshUtils)
endif()

include_directories(${NETGEN_INCLUDE_DIRS})
target_compile_definitions(NetgenPlugin PUBLIC NETGENPLUGIN_EXPORTS NETGEN_V6)

TARGET_INCLUDE_DIRECTORIES(NetgenPlugin PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/NetgenPlugin
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMesh
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMeshUtils
)
