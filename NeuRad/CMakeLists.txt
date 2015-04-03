# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/ExpertData/NeuRadData/
${CMAKE_SOURCE_DIR}/NeuRad/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ExpertNeuRad.cxx
ExpertNeuRadDigitizer.cxx
ExpertNeuRadPointAnalyser.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ExpertNeuRadLinkDef.h)
Set(LIBRARY_NAME NeuRad)
Set(DEPENDENCIES )

GENERATE_LIBRARY()

