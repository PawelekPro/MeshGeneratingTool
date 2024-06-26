ADD_LIBRARY(GeometryCore 
    Geometry/Geometry.cpp
    Geometry/TagMap.cpp
    QVTKProgressBar/ProgressBarPlugin.hpp
    GeometryImporter/GeometryImporter.cpp
    GeometryImporter/STEPImporter.cpp
    GeometryImporter/STLImporter.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Widgets
        GraphicalUtils
        )
else()
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Widgets
        GraphicalUtils
        )
endif()


TARGET_INCLUDE_DIRECTORIES(GeometryCore PUBLIC
    ${PRJ_SOURCE_DIR}/src/GeometryCore/Geometry
    ${PRJ_SOURCE_DIR}/src/GeometryCore/QVTKProgressBar
    ${PRJ_SOURCE_DIR}/src/GeometryCore/GeometryImporter
)