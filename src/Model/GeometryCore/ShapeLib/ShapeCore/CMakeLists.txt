add_subdirectory(OcafAttributes)
add_subdirectory(ShapeDocumentImport)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeRegistry)

add_library(ShapeCore
    ShapeCore.cpp
    OcafShapeCore.cpp
)

target_link_libraries(ShapeCore PUBLIC
    ${OCC_LIBRARIES}
    Shapes
    ShapeMap
    ShapeRegistry
    XmlShapeLibDrivers
    OcafAttributes
    ShapeDocumentImport
)

target_include_directories(ShapeCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ShapeSignals
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)