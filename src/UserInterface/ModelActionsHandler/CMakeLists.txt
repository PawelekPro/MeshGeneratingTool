ADD_LIBRARY(ModelActionsHandler
  ModelActionsHandler.cpp
)

TARGET_LINK_LIBRARIES(ModelActionsHandler PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Rendering
  Utils
  ModelInterface
  GeometryActionsHandler
)

target_include_directories(ModelActionsHandler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)