ADD_LIBRARY(Shared
    SelectionHandler.cpp
    DocumentHandler.cpp
    JsonParser.cpp
)

if(WIN32)
    TARGET_LINK_LIBRARIES(Shared PUBLIC 
    Qt${QT_VERSION_MAJOR}::Xml
    TreeItems
)
else()
    TARGET_LINK_LIBRARIES(Shared PUBLIC
    Qt${QT_VERSION_MAJOR}::Xml
    TreeItems
)
endif()


TARGET_INCLUDE_DIRECTORIES(Shared PUBLIC
    ${PRJ_SOURCE_DIR}/src/Shared
)