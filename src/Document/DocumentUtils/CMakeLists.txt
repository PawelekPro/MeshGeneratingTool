add_library(DocumentUtils
    JsonParser.cpp
    DocUtils.cpp
    DefaultsParser.cpp
    DocItemTypes.cpp
    ComboBoxTypes.cpp
)

target_link_libraries(DocumentUtils PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Xml
    AppConfig
)

target_include_directories(DocumentUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)