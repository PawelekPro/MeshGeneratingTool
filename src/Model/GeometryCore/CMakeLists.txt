if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

add_subdirectory(GeoDoc)
add_subdirectory(GeoShapeEditor)
add_subdirectory(GeoShapes)
add_subdirectory(GeoUtils)

add_library(GeometryCore
    Geometry.cpp
    GeometryActions.cpp
    GeometryView.cpp
)

target_link_libraries(GeometryCore PUBLIC
    ${OCC_LIBRARIES}
    GeoDoc
    GeoShapeEditor
    GeoShapes
)

target_include_directories(GeometryCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)