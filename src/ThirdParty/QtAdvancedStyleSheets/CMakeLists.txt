ADD_LIBRARY(QtAdvancedStyleSheets 
  src/QmlStyleUrlInterceptor.cpp
  src/QtAdvancedStylesheet.cpp
)

target_compile_definitions(QtAdvancedStyleSheets PUBLIC ACSS_SHARED_EXPORT)

TARGET_LINK_LIBRARIES(QtAdvancedStyleSheets PUBLIC 
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Qml
  Qt${QT_VERSION_MAJOR}::Svg
  
)

target_include_directories(QtAdvancedStyleSheets PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

