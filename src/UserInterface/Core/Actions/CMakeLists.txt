add_library(Actions
    GeometryActions.cpp
    MeshActions.cpp
)

target_link_libraries(Actions PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  ModelInterface
)

target_include_directories(Actions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)