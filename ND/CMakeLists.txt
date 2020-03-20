# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/BeamDet/data
${CMAKE_SOURCE_DIR}/data/
${CMAKE_SOURCE_DIR}/ND/
${CMAKE_SOURCE_DIR}/ND/data
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERND.cxx
ERNDDigitizer.cxx
ERNDTrackFinder.cxx
ERNDSetup.cxx
data/ERNDPoint.cxx
data/ERNDDigi.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERNDLinkDef.h)
Set(LIBRARY_NAME ND)
Set(DEPENDENCIES ERBase ERData Base Core)

GENERATE_LIBRARY()

