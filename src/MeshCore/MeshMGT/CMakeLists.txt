SET(MeshMGT_HEADERS
    MeshMGT_Macro.h
    MeshMGT_Mesh.h
    MeshMGT_MeshAttributes.h
    MeshMGT_ComputeError.hpp
    MeshMGT_Algorithm.h
    MeshMGT_SubMesh.h
)

SET(MeshMGT_SOURCES
    MeshMGT_Mesh.cpp
    MeshMGT_MeshAttributes.cpp
    MeshMGT_ComputeError.hpp
    MeshMGT_Algorithm.cpp
    MeshMGT_SubMesh.cpp
)

SET(_link_LIBRARIES
    MeshHDS
)

ADD_LIBRARY(MeshMGT ${MeshMGT_SOURCES})
TARGET_LINK_LIBRARIES(MeshMGT ${_link_LIBRARIES} )

TARGET_INCLUDE_DIRECTORIES(MeshMGT PUBLIC
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshHDS
)