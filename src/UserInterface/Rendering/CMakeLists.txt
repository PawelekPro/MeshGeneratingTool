ADD_LIBRARY(Rendering 
  QVTKInteractorStyle.cpp
  QVTKNavigationWidget.cpp
  QVTKRenderWindow.cpp
  QIVtkSelectionPipeline.cpp
)

INCLUDE_DIRECTORIES(
  ${PRJ_SOURCE_DIR}/src/UserInterface/Rendering
)

if(WIN32)
  TARGET_LINK_LIBRARIES(Rendering PUBLIC 
    ${OpenCASCADE_LIBRARIES}
    ${VTK_LIBRARIES}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    GeometryCore
    Model
)
else()
  TARGET_LINK_LIBRARIES(Rendering PUBLIC 
    ${OpenCASCADE_LIBRARIES}
    ${VTK_LIBRARIES}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    GeometryCore
    Model
)
endif()

TARGET_INCLUDE_DIRECTORIES(Rendering PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Rendering
)
