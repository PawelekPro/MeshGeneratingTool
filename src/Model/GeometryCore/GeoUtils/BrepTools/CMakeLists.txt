add_library(BrepTools 
    BrepTools.cpp
)

target_link_libraries(BrepTools PUBLIC
    ${OCC_LIBRARIES}
)

target_include_directories(BrepTools PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)