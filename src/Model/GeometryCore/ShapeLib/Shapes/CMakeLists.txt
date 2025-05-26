add_library(Shapes
    ShapeId.cpp
    IntPairKey.cpp
    )

target_link_libraries(Shapes PUBLIC
    ${OCC_LIBRARIES}
)

target_include_directories(Shapes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)