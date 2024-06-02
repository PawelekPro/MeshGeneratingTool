ADD_LIBRARY(Widgets 
  AbstractLineEdit.cpp
  IntLineWidget.cpp
  DoubleLineWidget.cpp
  ComboBoxWidget.cpp
  BaseWidget.h
  EntityPickWidget.cpp
  TreeContextMenu.cpp
  RibbonBarWidget.cpp
  ColorPickerWidget.cpp
  TreeWidgetEventHandler.cpp
  ${SARIBBON_PATH}/SARibbon.h
  ${SARIBBON_PATH}/SARibbon.cpp
  ${SARIBBON_PATH}/SARibbonBar/resource.qrc
)

INCLUDE_DIRECTORIES(
  ${PRJ_SOURCE_DIR}/src/UserInterface/Widgets
)

TARGET_LINK_LIBRARIES(Widgets PUBLIC 
  ${VTK_LIBRARIES}  
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Core5Compat
  GraphicalUtils
  Rendering
  Defaults
  Core
)

TARGET_INCLUDE_DIRECTORIES(Widgets PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/
    ${PRJ_SOURCE_DIR}/src/UserInterface/Widgets)

