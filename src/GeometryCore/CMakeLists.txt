ADD_LIBRARY(GeometryCore 
    Geometry/GeometryFunctions.cpp
    QVTKProgressBar/ProgressBarPlugin.hpp
    STEPPlugin/STEPPlugin.cpp
    STEPPlugin/STEPPluginOperations.cpp
    STLPlugin/STLPluginOperations.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(GeometryCore PRIVATE 
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        GraphicalUtils)
else()
    TARGET_LINK_LIBRARIES(GeometryCore PRIVATE
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        GraphicalUtils)
endif()


TARGET_INCLUDE_DIRECTORIES(GeometryCore PUBLIC
    ${PRJ_SOURCE_DIR}/src/GeometryCore/Geometry
    ${PRJ_SOURCE_DIR}/src/GeometryCore/QVTKProgressBar
    ${PRJ_SOURCE_DIR}/src/GeometryCore/STEPPlugin
    ${PRJ_SOURCE_DIR}/src/GeometryCore/STLPlugin
)