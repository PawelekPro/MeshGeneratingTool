add_library(OcafAttributes
    LabelPathAttr.cpp
    AssemblyPathAttr.cpp
    ShapePathAttr.cpp
)

target_link_libraries(OcafAttributes PUBLIC
    ${OCC_LIBRARIES}
    Shapes
)

target_include_directories(OcafAttributes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)