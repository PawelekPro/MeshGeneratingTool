add_subdirectory(GeometryCore)
add_subdirectory(MeshCore)
add_subdirectory(ModelInterface)
add_subdirectory(ModelEvents)
add_subdirectory(ModelCommands)

if (BUILD_TESTS)
    add_subdirectory(Tests)
endif()

ADD_LIBRARY(Model
    Model.cpp
    ModelManager.cpp
    ModelDocParser.cpp
)

TARGET_LINK_LIBRARIES(Model PUBLIC 
    spdlog::spdlog_header_only
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Document
    ModelEvents
)


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore/MGTMesh
)
