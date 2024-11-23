add_subdirectory(NetgenPlugin)
add_subdirectory(MGTMesh)

ADD_LIBRARY(MeshCore 
    Mesh.cpp
    MeshContainers.hpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(MeshCore PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        )
else()
    TARGET_LINK_LIBRARIES(MeshCore PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        )
endif()

TARGET_INCLUDE_DIRECTORIES(MeshCore PUBLIC
    ${PRJ_SOURCE_DIR}/src/MeshCore
)


