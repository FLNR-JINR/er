# Create a library called "libDet" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/BeamDet
${CMAKE_SOURCE_DIR}/base
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
    ERBeamDet.cxx
    ERBeamDetTOFPoint.cxx
    ERBeamDetMWPCPoint.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBeamDetLinkDef.h)
Set(LIBRARY_NAME Beamdet)
Set(DEPENDENCIES ERBase ERData Base Core Geom)

GENERATE_LIBRARY()