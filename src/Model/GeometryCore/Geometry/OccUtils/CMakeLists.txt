add_library(OccUtils
    OccProgressIndicator.cpp
)

target_link_libraries(OccUtils PUBLIC
    ${OCC_LIBRARIES}
    ModelEvents
)

target_include_directories(OccUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)