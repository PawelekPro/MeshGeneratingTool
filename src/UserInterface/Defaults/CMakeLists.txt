ADD_LIBRARY(Defaults 
  AppDefaults.cpp
  AppSettings.cpp
  AppDefaultColors.cpp
  AppTheme.cpp
)

TARGET_LINK_LIBRARIES(Defaults PUBLIC 
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  ${VTK_LIBRARIES}  
  QtAdvancedStyleSheets
)

TARGET_INCLUDE_DIRECTORIES(Defaults PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Defaults
)

target_compile_definitions(Defaults PUBLIC 
    STYLES_DIR="${STYLES_DIR}"
)