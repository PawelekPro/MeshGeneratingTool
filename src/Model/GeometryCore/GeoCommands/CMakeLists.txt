add_library(GeoCommands 
    ImportSTEPCommand.cpp
)

target_link_libraries(GeoCommands
    ModelCommands
    GeometryCore
    ${OCC_LIBRARIES}
)

target_include_directories(GeoCommands PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)