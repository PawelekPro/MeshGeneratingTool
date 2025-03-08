ADD_LIBRARY(RenderWindow 
  QVTKInteractorStyle.cpp
  QVTKRenderWindow.cpp
  QIVtkSelectionPipeline.cpp
  QIVtkViewRepresentation.cpp
  QVTKCameraOrientationWidget.cpp
)

target_link_libraries(RenderWindow PUBLIC
  ${OCC_LIBRARIES}
  ${VTK_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::OpenGLWidgets 
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  GeometryCore
  Defaults
  Model
  RenderingUtils
)

target_include_directories(RenderWindow PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)