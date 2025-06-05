add_subdirectory(OcafAttributes)
add_subdirectory(XmlShapeLibDrivers)

add_library(ShapeCore
    ShapeCore.cpp
    OcafShapeCore.cpp
    AttributeFactory.cpp
)

target_link_libraries(ShapeCore PUBLIC
    ${OCC_LIBRARIES}
    Shapes
    ShapeMap
    XmlShapeLibDrivers
    OcafAttributes
)

target_include_directories(ShapeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)