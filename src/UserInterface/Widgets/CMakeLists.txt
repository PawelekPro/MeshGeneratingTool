ADD_LIBRARY(Widgets 
  AbstractLineEdit.cpp
  AbstractLineEdit.h
)

TARGET_LINK_LIBRARIES(Widgets PRIVATE 
  Qt${QT_VERSION_MAJOR}::Widgets
)

TARGET_INCLUDE_DIRECTORIES(Widgets PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Widgets)