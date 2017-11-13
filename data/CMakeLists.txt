# Create a library called "libdata" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${Boost_INCLUDE_DIRS}
${ROOT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/data/NeuRadData
${CMAKE_SOURCE_DIR}/data/muSiData
${CMAKE_SOURCE_DIR}/data/NDData
${CMAKE_SOURCE_DIR}/data/beamtime
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERStack.cxx
ERMCTrack.cxx
ERMCEventHeader.cxx
ERDecayMCEventHeader.cxx
EREventHeader.cxx
ERLi10EventHeader.cxx
ERLi10MCEventHeader.cxx

ERHe8EventHeader.cxx
ERRawEvent.cxx
ERParticle.cxx
ERPoint.cxx
ERDigi.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS})
set(LINKDEF  ERDataLinkDef.h)
set(LIBRARY_NAME ERData)
set(DEPENDENCIES Base Core)

GENERATE_LIBRARY()
