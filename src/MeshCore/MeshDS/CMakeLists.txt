SET(MeshDS_HEADERS
    MeshDS_Macro.h
    MeshDS_MeshObject.h
    MeshDS_Mesh.h
)

SET(MeshDS_SOURCES
    MeshDS_MeshObject.cpp
    MeshDS_Mesh.cpp
)


ADD_LIBRARY(MeshDS ${MeshDS_SOURCES})
TARGET_LINK_LIBRARIES(MeshDS ${_link_LIBRARIES} )
