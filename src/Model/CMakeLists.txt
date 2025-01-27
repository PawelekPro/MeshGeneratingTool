add_subdirectory(GeometryCore)
add_subdirectory(MeshCore)
add_subdirectory(ModelInterface)

ADD_LIBRARY(Model
    Model.cpp
    ModelManager.cpp
    ModelDocParser.cpp
)


TARGET_LINK_LIBRARIES(Model PUBLIC 
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Document
)


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore/MGTMesh
)
