ADD_LIBRARY(Utils 
  ProgressBar.cpp
  FileDialogUtils.cpp
)

TARGET_LINK_LIBRARIES(Utils PUBLIC
  ${VTK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Core
)

TARGET_INCLUDE_DIRECTORIES(Utils PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/QVTKProgressBar
)

# Autogenerated uic files
if(WIN32)
  TARGET_INCLUDE_DIRECTORIES(Utils INTERFACE 
    ${CMAKE_CURRENT_BINARY_DIR}/Utils_autogen/include_Release
  )
else()
  TARGET_INCLUDE_DIRECTORIES(Utils INTERFACE 
    ${CMAKE_CURRENT_BINARY_DIR}/Utils_autogen/include
  )
endif()