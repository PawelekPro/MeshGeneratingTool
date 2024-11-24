ADD_LIBRARY(TreeItems
  TreeItem.cpp
  TreeItemFactory.cpp
)

TARGET_LINK_LIBRARIES(TreeItems PUBLIC
  Document
  TreeStructure
)

target_include_directories(TreeItems PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)