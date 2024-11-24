ADD_LIBRARY(TreeCommands
  TreeCommand.cpp
  AddTreeItemCommand.cpp
  RemoveTreeItemCommand.cpp
  TreeCommandManager.cpp
  )

TARGET_LINK_LIBRARIES(TreeCommands PUBLIC
  ${VTK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  TreeItems
  TreeStructure
)

TARGET_INCLUDE_DIRECTORIES(TreeCommands PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

