ADD_LIBRARY(MeshActionsHandler
  MeshActionsHandler.cpp
)

TARGET_LINK_LIBRARIES(MeshActionsHandler PUBLIC
    spdlog::spdlog_header_only
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    MeshCommands
    Utils
    ModelInterface
    TreeStructure
)

target_include_directories(MeshActionsHandler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)