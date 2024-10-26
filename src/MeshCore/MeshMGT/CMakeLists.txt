SET(MeshMGT_HEADERS
    MeshMGT_Macro.h
    MeshMGT_Mesh.h
    MeshMGT_MeshAttributes.h
)

SET(MeshMGT_SOURCES
    MeshMGT_Mesh.cpp
    MeshMGT_MeshAttributes.cpp
)

SET(_link_LIBRARIES
    MeshHDS
)

ADD_LIBRARY(MeshMGT ${MeshMGT_SOURCES})
TARGET_LINK_LIBRARIES(MeshMGT ${_link_LIBRARIES} )

TARGET_INCLUDE_DIRECTORIES(MeshMGT PUBLIC
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshHDS
)