ADD_SUBDIRECTORY(Core)
ADD_SUBDIRECTORY(Defaults)
ADD_SUBDIRECTORY(TreeStructure)
ADD_SUBDIRECTORY(Rendering)
ADD_SUBDIRECTORY(Widgets)
ADD_SUBDIRECTORY(Utils)

ADD_LIBRARY(UserInterface)

TARGET_LINK_LIBRARIES(UserInterface PUBLIC 
  ${VTK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Core
  Widgets
  Defaults
  TreeStructure
  Rendering
  Core
  Utils
)