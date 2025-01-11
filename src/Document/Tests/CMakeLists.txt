add_executable(utDocument
    utRun.cpp
    utDocItemTypes.cpp
    utDocumentHandler.cpp
    utProperties.cpp
    utDefaultsParser.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(utDocument PUBLIC
    GTest::GTest 
    GTest::Main
    Document
    DocumentUtils
)

include(GoogleTest)
gtest_discover_tests(utDocument)