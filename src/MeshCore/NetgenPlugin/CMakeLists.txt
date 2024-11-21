ADD_LIBRARY(NetgenPlugin 
    NetgenPlugin_NetgenLibWrapper.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(NetgenPlugin PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        ${NETGEN_LIBRARIES})
else()
    TARGET_LINK_LIBRARIES(NetgenPlugin PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        ${NETGEN_LIBRARIES})
endif()

include_directories(${NETGEN_INCLUDE_DIRS})
target_compile_definitions(NetgenPlugin PUBLIC NETGENPLUGIN_EXPORTS NETGEN_V6)

TARGET_INCLUDE_DIRECTORIES(NetgenPlugin PUBLIC
    ${PRJ_SOURCE_DIR}/src/NetgenPlugin
)
