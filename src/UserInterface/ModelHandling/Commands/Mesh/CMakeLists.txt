add_library(MeshCommands
  AddSizingCommand.cpp
)

target_link_libraries(MeshCommands PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Utils
    Document
    TreeStructure
    Commands
    RenderSignalSenders
)

target_include_directories(MeshCommands PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
