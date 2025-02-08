add_executable(utDocument
    utRun.cpp
    utDocItemTypes.cpp
    utDocumentHandler.cpp
    utProperties.cpp
    utDefaultsParser.cpp
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

find_package(GTest REQUIRED)

target_link_libraries(utDocument PUBLIC
    GTest::GTest 
    GTest::Main
    Document
    DocumentUtils
)

target_include_directories(utDocument PUBLIC ${CMAKE_SOURCE_DIR}/resources/resources.qrc)

MESSAGE(SYTEM "--------------PATH: ${CMAKE_SOURCE_DIR}/resources/resources.qrc")

qt_add_resources(utDocument
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

target_sources(utDocument PRIVATE ${utDocument_RESOURCES})

include(GoogleTest)
gtest_discover_tests(utDocument)