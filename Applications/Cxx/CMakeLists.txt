# Build the GDCM applications
# Namely:
# gdcmdump
# gdcminfo
# gdcmconv

CHECK_INCLUDE_FILE("fcntl.h"      CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("sched.h"      CMAKE_HAVE_SHED_H)
CHECK_INCLUDE_FILE("sys/mman.h"   CMAKE_HAVE_SYS_MMAN_H)

IF(WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES(
    "${GDCM_SOURCE_DIR}/Utilities/getopt"
  )
ENDIF(WIN32 AND NOT CYGWIN)

# Add the include paths
INCLUDE_DIRECTORIES(
  "${GDCM_BINARY_DIR}/Source/Common"
  "${GDCM_SOURCE_DIR}/Source/Common"
  "${GDCM_SOURCE_DIR}/Source/DataStructureAndEncodingDefinition"
  "${GDCM_SOURCE_DIR}/Source/MediaStorageAndFileFormat"
  "${GDCM_SOURCE_DIR}/Source/InformationObjectDefinition"
# FIXME:
  "${GDCM_SOURCE_DIR}/Source/DataDictionary"
  "${GDCM_BINARY_DIR}/Utilities/zlib"
  "${GDCM_SOURCE_DIR}/Utilities"
  )

IF(WIN32)
  IF (BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DGETOPT_DLL)
  ENDIF (BUILD_SHARED_LIBS)
ENDIF(WIN32)

SET(GDCM_EXECUTABLE_NAME
  gdcmdump
  gdcmraw
  gdcmscanner
  #gdcmoverlay
  gdcmimg
  #deflate
  gdcmconv
  gdcminfo
  )
# Need mmap
#IF(CMAKE_HAVE_SYS_MMAN_H)
#  SET(GDCM_EXECUTABLE_NAME ${GDCM_EXECUTABLE_NAME}
#    gdcmmmap
#    gdcmfile
#  )
#ENDIF(CMAKE_HAVE_SYS_MMAN_H)

FOREACH(exename ${GDCM_EXECUTABLE_NAME})
  ADD_EXECUTABLE(${exename} ${exename}.cxx)
  SET_TARGET_PROPERTIES(${exename} PROPERTIES ${GDCM_EXECUTABLE_PROPERTIES})
  IF(${exename} STREQUAL "gdcminfobla")
    TARGET_LINK_LIBRARIES(${exename} gdcmDSED gdcmDICT)
  ELSE(${exename} STREQUAL "gdcminfobla")
    TARGET_LINK_LIBRARIES(${exename} gdcmMSFF)
  ENDIF(${exename} STREQUAL "gdcminfobla")
  IF(WIN32 AND NOT CYGWIN)
    TARGET_LINK_LIBRARIES(${exename} gdcmgetopt)
  ENDIF(WIN32 AND NOT CYGWIN)
  INSTALL(TARGETS ${exename}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static
	)
ENDFOREACH(exename)

#ADD_EXECUTABLE(deflate deflate.cxx)
#TARGET_LINK_LIBRARIES(deflate gdcmzlib)
