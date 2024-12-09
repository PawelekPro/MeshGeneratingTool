<<<<<<< HEAD
ADD_LIBRARY(Rendering 
  QVTKInteractorStyle.cpp
  QVTKRenderWindow.cpp
  QIVtkSelectionPipeline.cpp
  QIVtkViewRepresentation.cpp
  QIVtkUtils.cpp
  QVTKCameraOrientationWidget.cpp
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
    Defaults
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
    Defaults
    Model
)
endif()

TARGET_INCLUDE_DIRECTORIES(Rendering PUBLIC
    ${PRJ_SOURCE_DIR}/src/UserInterface/Rendering
)
=======
add_subdirectory(RenderWindow)
add_subdirectory(RenderingUtils)
<<<<<<< HEAD
add_subdirectory(RenderingSignalHandlers)
>>>>>>> 4e969e1 ([WIP] added working render signal receiver)
=======
add_subdirectory(RenderSignalHandlers)
>>>>>>> d50f60d ([WIP] compilable finalized ui structure - needs implementation)
