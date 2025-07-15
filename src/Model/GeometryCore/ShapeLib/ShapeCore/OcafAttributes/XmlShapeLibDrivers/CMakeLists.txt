add_library(XmlShapeLibDrivers
    XmlShapeLibDrivers.cpp
    XmlShapeLibDrivers_ShapeIdDriver.cpp
)

target_link_libraries(XmlShapeLibDrivers PUBLIC
    ${OCC_LIBRARIES}
    OcafAttributes
)

target_include_directories(XmlShapeLibDrivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)