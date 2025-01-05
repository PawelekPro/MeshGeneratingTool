add_library(RenderSignalHandlers
  RenderSignalHandler.cpp
  GeometryRenderHandler.cpp
  MeshRenderHandler.cpp
)

target_link_libraries( RenderSignalHandlers PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  RenderWindow
  ModelInterface
  GeometryActionsHandler
  MeshActionsHandler
  Utils
)

target_include_directories(RenderSignalHandlers PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)