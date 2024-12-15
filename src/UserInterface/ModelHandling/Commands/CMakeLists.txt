ADD_SUBDIRECTORY(Geometry)
ADD_SUBDIRECTORY(Mesh)

ADD_LIBRARY(Commands 
  CommandManager.cpp
)

TARGET_LINK_LIBRARIES(Commands PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  ModelInterface
  GeometryCommands
  Utils
  RenderSignalSenders
)

target_include_directories(Commands PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)