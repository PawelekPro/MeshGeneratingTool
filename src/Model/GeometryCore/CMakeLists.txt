add_subdirectory(GeoCommands)
add_subdirectory(GeoEvents)
add_subdirectory(Geometry/ShapeLib)
add_subdirectory(Geometry/GeoDoc)
add_subdirectory(Geometry/OccUtils)

ADD_LIBRARY(GeometryCore 
    # GeometryImporter/GeometryImporter.cpp
    # GeometryImporter/STEPImporter.cpp
    # GeometryImporter/STLImporter.cpp
    Geometry/OcafDoc.cpp
    Geometry/Geometry.cpp
    Geometry/TagMap.cpp
    # GeometryImporter/OccProgressWrapper.cpp
    Geometry/GeoService.cpp
    Geometry/GeoState.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        Utils
        ModelEvents
        ModelCommands
        GeoCommands
        GeoEvents
        OccUtils
        ShapeLib
        )
else()
    TARGET_LINK_LIBRARIES(GeometryCore PUBLIC
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        Utils
        ModelEvents
        )
endif()

TARGET_INCLUDE_DIRECTORIES(GeometryCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/GeometryImporter
)