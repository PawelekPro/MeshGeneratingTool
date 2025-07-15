add_library(ShapeDocumentImport
    OcafShapeDocumentImport.cpp
)

target_link_libraries(ShapeDocumentImport PUBLIC
    ${OCC_LIBRARIES}
    OcafAttributes
    ShapeRegistry
    Shapes
)

target_include_directories(ShapeDocumentImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)