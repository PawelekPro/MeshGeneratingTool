ADD_LIBRARY(GeometryImport 
    OcafImporter.cpp
    STEPOcafImporter.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GeometryImport PUBLIC
        ${OpenCASCADE_LIBRARIES}
        )
else()
    TARGET_LINK_LIBRARIES(GeometryImport PUBLIC
        ${OCC_LIBRARIES}
        )
endif()

TARGET_INCLUDE_DIRECTORIES(GeometryImport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)