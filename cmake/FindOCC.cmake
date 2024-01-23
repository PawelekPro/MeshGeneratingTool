# Try to find OCC
# Once done this will define
#
# OCC_FOUND          - system has OCC - OpenCASCADE
# OCC_INCLUDE_DIR    - where the OCC include directory can be found
# OCC_LIBRARY_DIR    - where the OCC library directory can be found
# OCC_LIBRARIES      - Link this to use OCC

############################################################################
#
# Modifications Copyright (C) 2018 Seth Hillbrand
#
# Based on FindOpenCasCade.cmake by the FreeCAD CAx development team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

# Set the needed libraries
set(OCC_LIBS
    TKBinL
    TKBin
    TKBinTObj
    TKBinXCAF
    TKBool
    TKBO
    TKBRep
    TKCAF
    TKCDF
    TKernel
    TKFeat
    TKFillet
    TKG2d
    TKG3d
    TKGeomAlgo
    TKGeomBase
    TKHLR
    TKIGES
    TKLCAF
    TKMath
    TKMesh
    TKMeshVS
    TKOffset
    TKOpenGl
    TKPrim
    TKService
    TKShHealing
    TKSTEP209
    TKSTEPAttr
    TKSTEPBase
    TKSTEP
    TKSTL
    TKTObj
    TKTopAlgo
    TKV3d
    TKVRML
    TKXCAF
    TKXDEIGES
    TKXDESTEP
    TKXMesh
    TKXmlL
    TKXml
    TKXmlTObj
    TKXmlXCAF
    TKXSBase
    TKIVtk
)

set(OCC_TYPE "OpenCASCADE Standard Edition")
if(WIN32)
  if(CYGWIN OR MINGW)
    FIND_PATH(OCC_INCLUDE_DIR Standard_Version.hxx
      /mingw64/include/opencascade
      /usr/include/opencascade
      /usr/local/include/opencascade
      /opt/opencascade/include
      /opt/opencascade/inc
    )
    message( "OCC ${OCC_INCLUDE_DIR}" )
    FIND_LIBRARY(OCC_LIBRARY TKernel
      HINTS
      ${OCC_LIBRARY_DIR}
      /usr/lib
      /mingw64/lib
      /usr/local/lib
      /opt/opencascade/lib
    )
    message( "OCC ${OCC_LIBRARY}" )
  else(CYGWIN OR MINGW)
    FIND_PATH(OCC_INCLUDE_DIR Standard_Version.hxx
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\SIM\\OCC\\2;Installation Path]/include"
    )
    FIND_LIBRARY(OCC_LIBRARY TKernel
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\SIM\\OCC\\2;Installation Path]/lib"
    )
  endif(CYGWIN OR MINGW)
else(WIN32)
  FIND_PATH(OCC_INCLUDE_DIR Standard_Version.hxx
    /usr/include/opencascade
    /usr/local/include/opencascade
    /opt/opencascade/include
    /opt/opencascade/inc
  )
  FIND_LIBRARY(OCC_LIBRARY TKernel
    HINTS
    ${OCC_LIBRARY_DIR}
    /usr
    /usr/local
    /opt/opencascade
    /opt/opencascade/lin64/gcc
    PATH_SUFFIXES lib
  )
endif(WIN32)


if(OCC_LIBRARY)
  GET_FILENAME_COMPONENT(OCC_LIBRARY_DIR ${OCC_LIBRARY} PATH)
  IF(NOT OCC_INCLUDE_DIR)
    FIND_PATH(OCC_INCLUDE_DIR Standard_Version.hxx
      ${OCC_LIBRARY_DIR}/../inc
    )
  ENDIF()
else(OCC_LIBRARY)
  message( "" )
  message( "*** OpenCascade library missing ***" )
  message( "Verify your OpenCascade installation or pass CMake" )
  message( "  the library directory as '-DOCC_LIBRARY_DIR=<path>'" )
  message( "" )
  message( FATAL_ERROR "" )
endif(OCC_LIBRARY)

if(OCC_INCLUDE_DIR AND NOT ${OCC_INCLUDE_DIR} STREQUAL "OCC_INCLUDE_DIR-NOTFOUND")
  file(STRINGS ${OCC_INCLUDE_DIR}/Standard_Version.hxx OCC_MAJOR
    REGEX "#define OCC_VERSION_MAJOR.*"
  )
  string(REGEX MATCH "[0-9]+" OCC_MAJOR ${OCC_MAJOR})
  file(STRINGS ${OCC_INCLUDE_DIR}/Standard_Version.hxx OCC_MINOR
    REGEX "#define OCC_VERSION_MINOR.*"
  )
  string(REGEX MATCH "[0-9]+" OCC_MINOR ${OCC_MINOR})
  file(STRINGS ${OCC_INCLUDE_DIR}/Standard_Version.hxx OCC_MAINT
    REGEX "#define OCC_VERSION_MAINTENANCE.*"
  )
  string(REGEX MATCH "[0-9]+" OCC_MAINT ${OCC_MAINT})

  set(OCC_VERSION_STRING "${OCC_MAJOR}.${OCC_MINOR}.${OCC_MAINT}")
else(OCC_INCLUDE_DIR AND NOT ${OCC_INCLUDE_DIR} STREQUAL "OCC_INCLUDE_DIR-NOTFOUND")
  message( "" )
  message( "*** OpenCascade header files missing ***" )
  message( "Verify your OpenCascade installation or pass CMake" )
  message( "  the header directory as '-DOCC_INCLUDE_DIR=<path>'" )
  message( "" )
  message( FATAL_ERROR "" )
endif(OCC_INCLUDE_DIR AND NOT ${OCC_INCLUDE_DIR} STREQUAL "OCC_INCLUDE_DIR-NOTFOUND")

# handle the QUIETLY and REQUIRED arguments and set OCC_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OCC REQUIRED_VARS OCC_INCLUDE_DIR VERSION_VAR OCC_VERSION_STRING)

if(OCC_FOUND)
  foreach(lib IN LISTS OCC_LIBS)

#Use the specified library location if given
    find_library(OCC_TEMP_LIB ${lib} HINTS ${OCC_LIBRARY_DIR} NO_DEFAULT_PATH)
    if(${OCC_TEMP_LIB} STREQUAL "OCC_TEMP_LIB-NOTFOUND")
      message( "" )
      message( "*** OpenCascade library missing ***" )
      message( "Could not find a library for ${lib} at ${OCC_LIBRARY_DIR}" )
      message( "Verify your OpenCascade installation or pass CMake" )
      message( "  the library directory as '-DOCC_LIBRARY_DIR=<path>'" )
      message( "" )
      message( FATAL_ERROR "" )
    else(${OCC_TEMP_LIB} STREQUAL "OCC_TEMP_LIB-NOTFOUND")
      list(APPEND OCC_LIBRARIES ${OCC_TEMP_LIB})
    endif(${OCC_TEMP_LIB} STREQUAL "OCC_TEMP_LIB-NOTFOUND")

    unset(OCC_TEMP_LIB CACHE)
  endforeach(lib)

  #Convert path names to absolute for cleaner display
  get_filename_component(OCC_INCLUDE_DIR "${OCC_INCLUDE_DIR}" ABSOLUTE)
  get_filename_component(OCC_LIBRARY_DIR "${OCC_LIBRARY_DIR}" ABSOLUTE)
  message(STATUS "Found ${OCC_TYPE} version: ${OCC_VERSION_STRING}")
  message(STATUS " ++ ${OCC_TYPE} include directory: ${OCC_INCLUDE_DIR}")
  message(STATUS " ++ ${OCC_TYPE} shared libraries directory: ${OCC_LIBRARY_DIR}")
endif(OCC_FOUND)