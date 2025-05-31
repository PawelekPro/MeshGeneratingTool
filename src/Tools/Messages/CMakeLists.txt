add_library(Messages INTERFACE)

target_include_directories(Messages INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_DIR}
)