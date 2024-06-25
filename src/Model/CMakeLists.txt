ADD_LIBRARY(Model 
    Model.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(Model PUBLIC 
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Utils
)
else()
    TARGET_LINK_LIBRARIES(Model PUBLIC
    ${GMSH_LIB}
    GeometryCore
    MeshCore
    Utils
)
endif()


TARGET_INCLUDE_DIRECTORIES(Model PUBLIC
    ${PRJ_SOURCE_DIR}/src/Model
)