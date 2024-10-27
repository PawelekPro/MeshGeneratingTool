SET(NetgenPlugin_HEADERS
  NetgenPlugin_Macro.h
  NetgenPlugin_Mesher.h
  NetgenPlugin_ngMeshInfo.h
  NetgenPlugin_LibWrapper.h
  NetgenPlugin_Internals.h
  
)

SET(NetgenPlugin_SOURCES
  NetgenPlugin_Mesher.cpp
  NetgenPlugin_ngMeshInfo.cpp
  NetgenPlugin_LibWrapper.cpp
  NetgenPlugin_Internals.cpp
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
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshHDS
    ${PRJ_SOURCE_DIR}/src/MeshCore/MeshDS
)