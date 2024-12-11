ADD_SUBDIRECTORY(Geometry)
ADD_SUBDIRECTORY(Mesh)

ADD_LIBRARY(ModelCommands 
  ModelCommandManager.cpp
)

TARGET_LINK_LIBRARIES(ModelCommands PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  ModelInterface
  GeometryCommands
  Utils
  RenderSignalSenders
)

target_include_directories(ModelCommands PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)