if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

ADD_LIBRARY(GeometryCore 
    GeometryImporter/GeometryImporter.cpp
    GeometryImporter/STEPImporter.cpp
    GeometryImporter/STLImporter.cpp
    Geometry/OcafDoc.cpp
    Geometry/Geometry.cpp
    Geometry/TagMap.cpp
)

TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
    ${OCC_LIBRARIES}
    ${VTK_LIBRARIES}
    Qt${QT_VERSION_MAJOR}::Widgets
    Utils
)

TARGET_INCLUDE_DIRECTORIES(GeometryCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/QVTKProgressBar
    ${CMAKE_CURRENT_SOURCE_DIR}/GeometryImporter
)