add_subdirectory(GeometryCore)
add_subdirectory(MeshCore)
add_subdirectory(ModelInterface)

ADD_LIBRARY(Model 
    Model.cpp
    ModelLifecycleManagers.cpp
)


if(WIN32)
    TARGET_LINK_LIBRARIES(Model PUBLIC 
    ${GMSH_LIB}
    GeometryCore
    MeshCore
)
else()
    TARGET_LINK_LIBRARIES(Model PUBLIC
    ${GMSH_LIB}
    GeometryCore
    MeshCore
)
endif()


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model
    )
