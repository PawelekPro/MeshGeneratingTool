add_subdirectory(Commands)
add_subdirectory(Messages)
add_subdirectory(Progress)
add_subdirectory(FileUtils)

add_library(Tools INTERFACE)

target_link_libraries(Tools INTERFACE
    BaseCommands
    Messages
    Progress
    FileUtils
)