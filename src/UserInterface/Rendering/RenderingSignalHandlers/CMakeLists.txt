add_library(RenderingSignalHandlers
  RenderSignalHandler.cpp
  BaseRenderHandler.cpp
  GeometryRenderHandler.cpp
  MeshRenderHandler.cpp
)

target_link_libraries( RenderingSignalHandlers PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  RenderWindow
  ModelInterface
  GeometryActionsHandler
  MeshActionsHandler
  Utils
)

target_include_directories(ModelActionsHandler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)