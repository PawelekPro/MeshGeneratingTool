# TODO: AppDefaultColors was a part of a singleton AppDefaults that held information 
# about app version etc. To decouple the UI from Document, the app metainfo like version 
# is now stored in Kernel/AppInfo. Perhaps we should have another singleton here like
# UIDefaults that would hold colors fonts or whatever we need. For now since it is just
# a small colors map i changed the AppDefaultColors to have a single static method

ADD_LIBRARY(Defaults
  AppDefaultColors.cpp
)

TARGET_LINK_LIBRARIES(Defaults PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
)

TARGET_INCLUDE_DIRECTORIES(Defaults PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)