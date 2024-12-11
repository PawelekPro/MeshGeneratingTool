add_subdirectory(Geometry)
add_subdirectory(Mesh)

ADD_LIBRARY(ModelActionsHandler
  ModelActionsHandler.cpp
)


TARGET_LINK_LIBRARIES(ModelActionsHandler PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Utils
  ModelInterface
  ModelCommands
  GeometryActionsHandler
  MeshActionsHandler
  RenderSignalSenders
)

target_include_directories(ModelActionsHandler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)