ADD_LIBRARY(MGTMeshUtils
        MGTMeshUtils_ControlPoint.cpp
        MGTMeshUtils_ViscousLayers.cpp
        MGTMeshUtils_ComputeError.hpp
        MGTMeshUtils_DefaultParameters.cpp
)


TARGET_LINK_LIBRARIES(MGTMeshUtils PUBLIC
    ${OCC_LIBRARIES}
    ${VTK_LIBRARIES}
)


include_directories()
