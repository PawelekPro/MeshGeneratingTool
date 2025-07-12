add_library(ShapeDocumentImport
    OcafShapeDocumentImport.cpp
    OcafShapeRegistry.cpp
)

target_link_libraries(ShapeDocumentImport PUBLIC
    ${OCC_LIBRARIES}
    OcafAttributes
    ShapeMap

)

target_include_directories(ShapeDocumentImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)