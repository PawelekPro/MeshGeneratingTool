ADD_LIBRARY(Widgets 
  AbstractLineEdit.cpp
  IntLineWidget.cpp
  DoubleLineWidget.cpp
  BaseWidget.h
)

TARGET_LINK_LIBRARIES(Widgets PUBLIC 
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Core5Compat
  GraphicalUtils
)

TARGET_INCLUDE_DIRECTORIES(Widgets PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Widgets)