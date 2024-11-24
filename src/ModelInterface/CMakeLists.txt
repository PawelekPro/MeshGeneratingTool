add_library(ModelInterface  
    ModelInterface.cpp
    ModelManager.cpp
)

target_link_libraries(ModelInterface PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Model
)

target_include_directories(ModelInterface PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
