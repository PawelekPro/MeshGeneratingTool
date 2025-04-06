ADD_LIBRARY(MGTMeshUtils
        MGTMeshUtils_ControlPoint.cpp
        MGTMeshUtils_ViscousLayers.cpp
        MGTMeshUtils_ComputeError.hpp
        MGTMeshUtils_DefaultParameters.cpp
        MGTMeshUtils_ProgressReporter.cpp
)

if (WIN32)
    TARGET_LINK_LIBRARIES(MGTMeshUtils PUBLIC
            ${OpenCASCADE_LIBRARIES}
            ${VTK_LIBRARIES})
else ()
    TARGET_LINK_LIBRARIES(MGTMeshUtils PUBLIC
            ${OCC_LIBRARIES}
            ${VTK_LIBRARIES})
endif ()

include_directories()
