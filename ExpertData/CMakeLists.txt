# Create a library called "libR3BDATA" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
${BASE_INCLUDE_DIRECTORIES} 
${R3BROOT_SOURCE_DIR}/ExpertData/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${CMAKE_SOURCE_DIR}/base
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
NeuRadData/ExpertNeuRadPoint.cxx
R3BMCStack.cxx
R3BMCTrack.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS})
set(LINKDEF  ExpertNeuRadLinkDef.h)
set(LIBRARY_NAME ExpertData)
set(DEPENDENCIES
    Base FairTools)

GENERATE_LIBRARY()
