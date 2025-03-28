# add_subdirectory(GeoImport)

add_library(GeoDoc
    XcafDoc.cpp
)

target_link_libraries(GeoDoc PUBLIC
    # GeoImport
    ModelEvents
)

target_include_directories(GeoDoc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)