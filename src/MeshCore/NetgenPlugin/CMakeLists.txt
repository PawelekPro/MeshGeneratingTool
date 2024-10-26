SET(NetgenPlugin_HEADERS
  NetgenPlugin_Macro.h
  NetgenPlugin_Mesher.h
  NetgenPlugin_ngMeshInfo.h
  NetgenPlugin_LibWrapper.h
)

SET(NetgenPlugin_SOURCES
  NetgenPlugin_Mesher.cpp
  NetgenPlugin_ngMeshInfo.cpp
  NetgenPlugin_LibWrapper.cpp
)

SET(_link_LIBRARIES
  ${Netgen_LIBRARIES}
  ${MeshMGT}
)

ADD_LIBRARY(NetgenPlugin ${NetgenPlugin_SOURCES})
TARGET_LINK_LIBRARIES(NetgenPlugin ${_link_LIBRARIES} )
SET_TARGET_PROPERTIES(NetgenPlugin PROPERTIES COMPILE_FLAGS "-DNETGENPLUGIN_EXPORTS -DNETGEN_V6")

TARGET_INCLUDE_DIRECTORIES(NetgenPlugin PUBLIC
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshMGT
)