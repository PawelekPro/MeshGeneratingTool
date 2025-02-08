ADD_LIBRARY(GeoImport 
    OcafImporter.cpp
    STEPOcafImporter.cpp
    STEPImporter.cpp
    STLImporter.cpp
    GeometryImporter.cpp
    )

if(WIN32)
    TARGET_LINK_LIBRARIES(GeoImport PUBLIC
        ${OpenCASCADE_LIBRARIES}
        GeoUtils
        )
else()
    TARGET_LINK_LIBRARIES(GeoImport PUBLIC
        ${OCC_LIBRARIES}
        )
endif()

TARGET_INCLUDE_DIRECTORIES(GeoImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)