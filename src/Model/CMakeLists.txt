ADD_LIBRARY(Model 
    Model.cpp
)

add_subdirectory(GeometryCore)
add_subdirectory(MeshCore)

if(WIN32)
    TARGET_LINK_LIBRARIES(Model PUBLIC 
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Shared
)
else()
    TARGET_LINK_LIBRARIES(Model PUBLIC
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Shared
)
endif()


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model
    )
