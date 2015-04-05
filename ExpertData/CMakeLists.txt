# Create a library called "libExpertData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/ExpertData/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
)

MESSAGE ("ROOT_INCLUDE_DIR = " ${ROOT_INCLUDE_DIR})
MESSAGE ("Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS})
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
NeuRadData/ExpertNeuRadPoint.cxx
NeuRadData/ExpertNeuRadDigi.cxx
NeuRadData/ExpertNeuRadStep.cxx
R3BMCStack.cxx
R3BMCTrack.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS})
set(LINKDEF  ExpertDataLinkDef.h)
set(LIBRARY_NAME ExpertData)
set(DEPENDENCIES
    Base)

GENERATE_LIBRARY()
