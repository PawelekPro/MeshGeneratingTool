ADD_LIBRARY(Defaults 
  AppDefaults.cpp
  AppSettings.cpp
  AppDefaultColors.cpp
  AppTheme.cpp
)

TARGET_LINK_LIBRARIES(Defaults PUBLIC 
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  QtAdvancedStyleSheets
)

TARGET_INCLUDE_DIRECTORIES(Defaults PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Defaults PUBLIC 
    STYLES_DIR="${STYLES_DIR}"
)