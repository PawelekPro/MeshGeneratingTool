ADD_SUBDIRECTORY(Core)
ADD_SUBDIRECTORY(Defaults)
ADD_SUBDIRECTORY(ModelHandling)
ADD_SUBDIRECTORY(TreeStructure)
ADD_SUBDIRECTORY(Rendering)
ADD_SUBDIRECTORY(Widgets)
ADD_SUBDIRECTORY(Utils)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

ADD_LIBRARY(UserInterface INTERFACE)

TARGET_LINK_LIBRARIES(UserInterface INTERFACE
  Core
  Defaults
  RenderWindow
  TreeStructure
  Utils
  Widgets
)