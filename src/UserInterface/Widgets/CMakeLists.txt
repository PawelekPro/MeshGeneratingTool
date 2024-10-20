ADD_LIBRARY(Widgets 
  WidgetFactory.cpp
  AbstractLineEdit.cpp
  IntLineWidget.cpp
  DoubleLineWidget.cpp
  ComboBoxWidget.cpp
  BaseWidget.h
  EntityPickWidget.cpp
  RibbonBarWidget.cpp
  ColorPickerWidget.cpp
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
  TreeStructure
  Rendering
  Defaults
  Core
)

TARGET_INCLUDE_DIRECTORIES(Widgets PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/
    ${PRJ_SOURCE_DIR}/src/UserInterface/Widgets)

