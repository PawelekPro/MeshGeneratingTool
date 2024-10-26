SET(MeshHDS_HEADERS
    MeshHDS_Macro.h
    MeshHDS_Mesh.h
    MeshHDS_Document.h
)

SET(MeshHDS_SOURCES
    MeshHDS_Mesh.cpp
    MeshHDS_Document.cpp
)

SET(_link_LIBRARIES
  MeshDS
)


ADD_LIBRARY(MeshHDS ${MeshHDS_SOURCES})
TARGET_LINK_LIBRARIES(MeshHDS ${_link_LIBRARIES} )

TARGET_INCLUDE_DIRECTORIES(MeshHDS PUBLIC
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshDS
)