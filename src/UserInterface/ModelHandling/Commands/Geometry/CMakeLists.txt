ADD_LIBRARY(GeometryCommands
  ImportGeometryCommand.cpp
)

TARGET_LINK_LIBRARIES(GeometryCommands PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Utils
    ModelInterface
    ModelCommands
)

target_include_directories(GeometryCommands PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)