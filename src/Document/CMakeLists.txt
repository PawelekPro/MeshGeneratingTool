add_subdirectory(DocumentUtils)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

add_library(Document
    DocumentHandler.cpp
)

target_link_libraries(Document PUBLIC
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Core
    DocumentUtils
    AppConfig
)

target_include_directories(Document PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    DocumentUtils
)

