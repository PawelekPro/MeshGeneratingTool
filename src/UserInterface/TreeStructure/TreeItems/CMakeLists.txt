ADD_LIBRARY(TreeItems
  TreeItem.cpp
  TreeItemFactory.cpp
  TreeItemTypes.cpp
)

INCLUDE_DIRECTORIES(
  ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeItems
  ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeCommands
  ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure
)

TARGET_LINK_LIBRARIES(TreeItems PUBLIC 
${VTK_LIBRARIES}
Qt${QT_VERSION_MAJOR}::Widgets
Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Gui
Qt${QT_VERSION_MAJOR}::Xml
Defaults
Widgets
Utils
Shared
TreeStructure
)

TARGET_INCLUDE_DIRECTORIES(TreeItems PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeItems
    ${PRJ_SOURCE_DIR}/src/UserInterface/TreeStructure/TreeCommands
    )

