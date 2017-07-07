# Create a library called "libERBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/passive
${CMAKE_SOURCE_DIR}/decayers
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERDetector.cxx
ERMCApplication.cxx
ERRunSim.cxx
ERSupport.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBaseLinkDef.h)
Set(LIBRARY_NAME ERBase)
Set(DEPENDENCIES Base Core ERDecayers)

GENERATE_LIBRARY()

