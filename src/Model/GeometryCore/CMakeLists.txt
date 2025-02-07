ADD_LIBRARY(GeometryCore 
    GeometryImporter/GeometryImporter.cpp
    GeometryImporter/STEPImporter.cpp
    GeometryImporter/STLImporter.cpp
    Geometry/Geometry.cpp
    Geometry/TagMap.cpp
    GeometryImporter/OccProgressWrapper.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Widgets
        Utils
        ModelEvents
        )
else()
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Widgets
        Utils
        ModelEvents
        )
endif()

TARGET_INCLUDE_DIRECTORIES(GeometryCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
    # ${CMAKE_CURRENT_SOURCE_DIR}/QVTKProgressBar
    ${CMAKE_CURRENT_SOURCE_DIR}/GeometryImporter
)