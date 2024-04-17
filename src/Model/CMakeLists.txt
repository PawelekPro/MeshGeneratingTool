ADD_LIBRARY(Model 
    Model.cpp
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