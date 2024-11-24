add_library(DocumentUtils
    JsonParser.cpp
)

target_link_libraries(DocumentUtils PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(DocumentUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)