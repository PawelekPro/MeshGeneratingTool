project(selectionAlgorithm)

add_executable(selectionAlgorithm 
    SelectionAlgorithm.cxx
)

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
    TARGET_LINK_LIBRARIES(selectionAlgorithm PRIVATE
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
        )
else()
    TARGET_LINK_LIBRARIES(selectionAlgorithm PRIVATE
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}
        )
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/geometrySample
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS selectionAlgorithm
  MODULES ${VTK_LIBRARIES}
)