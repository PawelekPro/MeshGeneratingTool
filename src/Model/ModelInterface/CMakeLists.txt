add_library(ModelInterface SHARED
    ModelInterface.cpp
    ModelDataView.cpp
)

target_link_libraries(ModelInterface PUBLIC
    spdlog::spdlog_header_only
    Model
    MeshCore
)

target_include_directories(ModelInterface PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)