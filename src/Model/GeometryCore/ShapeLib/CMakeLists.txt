add_subdirectory(Shapes)
add_subdirectory(ShapeMap)
add_subdirectory(ShapeTools)
add_subdirectory(ShapeCore)

add_library(ShapeLib 
    ShapeService.cpp
    ShapeView.cpp 
    ShapeSignalWrapper.cpp
)

target_link_libraries(ShapeLib PUBLIC
    Shapes
    ShapeMap
    ShapeTools
    ShapeCore
    Geometry_Events
    ${OCC_LIBRARIES}
    Messages
    Progress
)

target_include_directories(ShapeLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)