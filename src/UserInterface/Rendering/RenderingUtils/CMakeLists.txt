add_library(RenderingUtils 
    QIVtkUtils.cpp
)

target_link_libraries(RenderingUtils PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    ${VTK_LIBRARIES}
    Defaults
)

target_include_directories(RenderingUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)