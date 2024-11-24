add_library(AppConfig AppInfo.cpp)

target_link_libraries(AppConfig PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
)

TARGET_INCLUDE_DIRECTORIES(AppConfig PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)