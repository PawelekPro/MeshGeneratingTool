cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(meshGenerator)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

set(FORMS_PATH ${CMAKE_SOURCE_DIR}/src/UserInterface/Forms)
set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")
set(CMAKE_AUTOUIC_SEARCH_PATHS ${FORMS_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Project vars
######################################################################
MESSAGE("PROJECT NAME: ${PROJECT_NAME}")

SET ( PRJ_SOURCE_DIR ${PROJECT_SOURCE_DIR} )
SET ( PRJ_BINARY_DIR ${PROJECT_BINARY_DIR} )


# Include the src directory
add_subdirectory(src)


# Print info
######################################################################
STRING ( TOUPPER "${CMAKE_BUILD_TYPE}" MY_CMAKE_BUILD_TYPE )

MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CMAKE version       = ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" )
MESSAGE( STATUS "CMAKE_SYSTEM        = ${CMAKE_SYSTEM}" )
MESSAGE( STATUS "CMAKE_GENERATOR     = ${CMAKE_GENERATOR}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE    = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CMAKE_C_COMPILER    = ${CMAKE_C_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER  = ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "  FLAGS:" )
MESSAGE( STATUS )
MESSAGE( STATUS "CMAKE_C_FLAGS_${MY_CMAKE_BUILD_TYPE}          = ${CMAKE_C_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_${MY_CMAKE_BUILD_TYPE}        = ${CMAKE_CXX_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS_${MY_CMAKE_BUILD_TYPE} = ${CMAKE_EXE_LINKER_FLAGS_${MY_CMAKE_BUILD_TYPE}}" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "  MODULES:" )
MESSAGE( STATUS )

MESSAGE( STATUS "PRJ_SOURCE_DIR     = ${PRJ_SOURCE_DIR}" )
MESSAGE( STATUS "PRJ_BINARY_DIR	    = ${PRJ_BINARY_DIR}" )

MESSAGE( STATUS "-------------------------------------------------------------------------------" )


ADD_EXECUTABLE(meshGenerator
    ${GeometryCoreFiles} 
    ${UserInterfaceFiles}
    ${KernelFiles}
    ${RESOURCES_PATH}
)

TARGET_LINK_LIBRARIES (meshGenerator PRIVATE
    GeometryCore
    UserInterface
    Kernel
) 