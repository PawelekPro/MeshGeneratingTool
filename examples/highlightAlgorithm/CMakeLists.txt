project(highlightAlgorithm)

add_executable(highlightAlgorithm 
    HighlightAlgorithm.cxx
    IVtk_CustomSelectionPipeline.cxx
    IVtk_CustomInteractorStyle.cxx
)

find_package(TCL REQUIRED)
include_directories(
    ${TK_INCLUDE_PATH}
    ${TCL_INCLUDE_PATH})

if(WIN32)
    find_package(OpenCASCADE REQUIRED)
    if(OpenCASCADE_INCLUDE_DIR) 
        include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR})
        link_directories(${OpenCASCADE_LIBRARY_DIR})
    endif()
else()
    find_package(OCC REQUIRED)
    set(OCC_INCLUDE_DIR "/opt/opencascade/include/opencascade")
    if(OCC_INCLUDE_DIR) 
        include_directories(SYSTEM ${OCC_INCLUDE_DIR})
        link_directories(${OCC_LIBRARY_DIR})
    endif()
endif()

if(WIN32)
    TARGET_LINK_LIBRARIES(highlightAlgorithm PRIVATE
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        ${TK_LIBRARY}
        ${TCL_LIBRARY}
    )
else()
    TARGET_LINK_LIBRARIES(highlightAlgorithm PRIVATE
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        ${TK_LIBRARY}
        ${TCL_LIBRARY}
    )
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/geometrySample
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
