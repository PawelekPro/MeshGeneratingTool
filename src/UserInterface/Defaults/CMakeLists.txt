ADD_LIBRARY(Defaults 
  AppDefaults.cpp
)

TARGET_LINK_LIBRARIES(Defaults PRIVATE 
  Qt${QT_VERSION_MAJOR}::Core
  ${VTK_LIBRARIES}  
)

TARGET_INCLUDE_DIRECTORIES(Defaults PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Defaults
)