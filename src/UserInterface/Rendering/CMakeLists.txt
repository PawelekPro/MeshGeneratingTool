ADD_LIBRARY(Rendering 
  QVTKInteractorStyle.cpp
  QVTKInteractorStyle.h
  QVTKNavigationWidget.cpp
  QVTKNavigationWidget.h
  QVTKRenderWindow.cpp
  QVTKRenderWindow.h
  QVTKToolBar.cpp
  QVTKToolBar.h
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
)
endif()

TARGET_INCLUDE_DIRECTORIES(Rendering PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Rendering
)
