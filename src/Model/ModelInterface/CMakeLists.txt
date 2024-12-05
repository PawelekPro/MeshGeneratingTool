add_library(ModelInterface 
    ModelInterface.cpp
)

target_link_libraries(ModelInterface PUBLIC
    Model
)

target_include_directories(ModelInterface PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)