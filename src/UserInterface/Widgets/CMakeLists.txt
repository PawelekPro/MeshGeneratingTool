ADD_LIBRARY(Widgets 
  WidgetFactory.cpp
  AbstractLineEdit.cpp
  IntLineWidget.cpp
  DoubleLineWidget.cpp
  ComboBoxWidget.cpp
  BaseWidget.hpp
  EntityPickWidget.cpp
  RibbonBarWidget.cpp
  ColorPickerWidget.cpp
  ${SARIBBON_PATH}/SARibbon.h
  ${SARIBBON_PATH}/SARibbon.cpp
  ${SARIBBON_PATH}/SARibbonBar/resource.qrc
)

TARGET_LINK_LIBRARIES(Widgets PUBLIC 
  ${VTK_LIBRARIES}  
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Core5Compat
  AppConfig
  DocumentUtils
  TreeStructure
  Core
)

TARGET_INCLUDE_DIRECTORIES(Widgets PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

