ADD_SUBDIRECTORY(Config)

ADD_LIBRARY(Kernel 
  AppQuickStart.cpp
)

TARGET_LINK_LIBRARIES(Kernel PRIVATE 
  spdlog::spdlog_header_only
  ${VTK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Widgets
  UserInterface
  Model
)

TARGET_INCLUDE_DIRECTORIES(Kernel PUBLIC
    ${PRJ_SOURCE_DIR}/src/Kernel
)