add_executable(ocafExample ocafMain.cpp)


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
    TARGET_LINK_LIBRARIES(ocafExample PRIVATE
        ${OpenCASCADE_LIBRARIES}
        ${VTK_LIBRARIES}
    )
else()
    TARGET_LINK_LIBRARIES(ocafExample PRIVATE
        ${OCC_LIBRARIES}
        ${VTK_LIBRARIES}

    )
endif()
