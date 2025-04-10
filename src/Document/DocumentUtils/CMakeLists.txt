add_library(DocumentUtils
        DocUtils.cpp
        DefaultsParser.cpp
        DocItemTypes.cpp
        ComboBoxTypes.cpp
        DisplayRulesParser.cpp
)

target_link_libraries(DocumentUtils PUBLIC
        spdlog::spdlog_header_only
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Xml
        AppConfig
)

target_include_directories(DocumentUtils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)