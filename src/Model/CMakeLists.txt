add_subdirectory(GeometryCore)
add_subdirectory(MeshCore)
add_subdirectory(ModelInterface)
add_subdirectory(ModelEvents)

ADD_LIBRARY(Model
        Model.cpp
        ModelManager.cpp
        ModelDocParser.cpp
)


TARGET_LINK_LIBRARIES(Model PUBLIC
        spdlog::spdlog_header_only
        GeometryCore
        MeshCore
        Document
)

if (ENABLE_GMSH)
    TARGET_LINK_LIBRARIES(Model PUBLIC
            ${GMSH_LIB})
endif ()


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore
        ${CMAKE_CURRENT_SOURCE_DIR}/MeshCore/MGTMesh
)
