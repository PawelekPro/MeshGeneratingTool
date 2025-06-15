add_subdirectory(Commands)
add_subdirectory(Events)
add_subdirectory(ShapeLib)

add_library(GeometryCore
    Geometry.cpp
)

target_link_libraries(GeometryCore PUBLIC
    ${OCC_LIBRARIES}
    ShapeLib
    Geometry_Events
    Geometry_Commands
    BaseCommands
    Messages
    Progress
)

target_include_directories(GeometryCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)