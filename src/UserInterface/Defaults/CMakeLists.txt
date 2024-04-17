ADD_LIBRARY(Defaults 
  AppDefaults.hpp
)

TARGET_LINK_LIBRARIES(Defaults PRIVATE 
  Qt${QT_VERSION_MAJOR}::Core
)

TARGET_INCLUDE_DIRECTORIES(Defaults PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Defaults
)