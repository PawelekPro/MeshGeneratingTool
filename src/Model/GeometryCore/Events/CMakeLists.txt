add_library(Geometry_Events INTERFACE)

target_link_libraries(Geometry_Events INTERFACE
    Messages 
    Shapes
    ${OCC_LIBRARIES}
)

message(STATUS "${BOOST_DIR}")

target_include_directories(Geometry_Events INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)