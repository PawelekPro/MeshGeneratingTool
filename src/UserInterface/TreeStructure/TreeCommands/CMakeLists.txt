ADD_LIBRARY(TreeCommands
  AddTreeItemCommand.cpp
  TreeCommand.cpp
  )

TARGET_LINK_LIBRARIES(TreeCommands PUBLIC
${VTK_LIBRARIES}
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Gui
TreeItems
TreeStructure
)

TARGET_INCLUDE_DIRECTORIES(TreeCommands PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeCommands
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeItems
    )

