ADD_LIBRARY(GeometryActionsHandler
  GeometryActionsHandler.cpp
)

TARGET_LINK_LIBRARIES(GeometryActionsHandler PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Utils
    ModelInterface
    ModelCommands
    RenderSignalSenders
)

target_include_directories(GeometryActionsHandler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)