ADD_LIBRARY(qtadvancedcss 
  QmlStyleUrlInterceptor.cpp
  QtAdvancedStylesheet.cpp
)

TARGET_LINK_LIBRARIES(qtadvancedcss PUBLIC 
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Qml
  Qt${QT_VERSION_MAJOR}::Svg
  
)

target_include_directories(qtadvancedcss PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
