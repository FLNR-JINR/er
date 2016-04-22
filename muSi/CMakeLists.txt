# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/ERData/muSiData/
${CMAKE_SOURCE_DIR}/ERData/
${CMAKE_SOURCE_DIR}/muSi/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERmuSi.cxx
ERmuSiHitProducer.cxx
ERmuSiTrackFinder.cxx
ERmuSiMatcher.cxx
ERmuSiVertexFinder.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERmuSiLinkDef.h)
Set(LIBRARY_NAME muSi)
Set(DEPENDENCIES ERData Minuit Base Core)

GENERATE_LIBRARY()

