add_library(Geometry_Commands 
    ImportSTEPCommand.cpp
    GeoCommandsFactory.cpp
    ShapeLibCommand.cpp
)

target_link_libraries(Geometry_Commands
    BaseCommands 
    ShapeLib 
    ${OCC_LIBRARIES}
)

target_include_directories(Geometry_Commands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)