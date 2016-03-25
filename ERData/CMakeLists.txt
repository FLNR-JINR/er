# Create a library called "libERData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/ERData
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
NeuRadData/ERNeuRadPoint.cxx
NeuRadData/ERNeuRadFiberPoint.cxx
NeuRadData/ERNeuRadPMTSignal.cxx
NeuRadData/ERNeuRadDigi.cxx
NeuRadData/ERNeuRadStep.cxx
muSiData/ERmuSiPoint.cxx
muSiData/ERmuSiHit.cxx
GadastData/ERGadastCsIPoint.cxx
GadastData/ERGadastLaBrPoint.cxx
GadastData/ERGadastStep.cxx
ERStack.cxx
ERMCTrack.cxx
ERMCEventHeader.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS})
set(LINKDEF  ERDataLinkDef.h)
set(LIBRARY_NAME ERData)
set(DEPENDENCIES
    Base)

GENERATE_LIBRARY()
