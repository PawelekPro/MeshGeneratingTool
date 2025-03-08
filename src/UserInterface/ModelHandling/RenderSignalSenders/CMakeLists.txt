add_library(RenderSignalSenders
    RenderSignalSender.cpp
    GeometrySignalSender.cpp
    MeshSignalSender.cpp
)

target_include_directories(RenderSignalSenders PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PRJ_SOURCE_DIR}/src/Model/MeshCore/MGTMesh
)

target_link_libraries(RenderSignalSenders PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    ${OCC_LIBRARIES}
    ${VTK_LIBRARIES}
)

