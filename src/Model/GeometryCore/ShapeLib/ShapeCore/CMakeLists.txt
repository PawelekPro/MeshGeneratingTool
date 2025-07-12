add_subdirectory(OcafAttributes)
add_subdirectory(XmlShapeLibDrivers)
add_subdirectory(ShapeDocumentImport)

add_library(ShapeCore
    ShapeCore.cpp
    OcafShapeCore.cpp
)

target_link_libraries(ShapeCore PUBLIC
    ${OCC_LIBRARIES}
    Shapes
    ShapeMap
    XmlShapeLibDrivers
    OcafAttributes
    ShapeDocumentImport
)

target_include_directories(ShapeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ShapeSignals
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)