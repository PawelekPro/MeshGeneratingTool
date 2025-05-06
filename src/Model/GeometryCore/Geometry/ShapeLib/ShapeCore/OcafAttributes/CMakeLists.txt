add_library(OcafAttributes
    ShapeIdAttribute.cpp
)

target_link_libraries(OcafAttributes PUBLIC
    ${OCC_LIBRARIES}
    Shapes
)

target_include_directories(OcafAttributes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)