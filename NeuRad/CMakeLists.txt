# Create a library called "libNeuRad" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/ERData/NeuRadData/
${CMAKE_SOURCE_DIR}/ERData/
${CMAKE_SOURCE_DIR}/NeuRad/
${CMAKE_SOURCE_DIR}/ERBase/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}

) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERNeuRad.cxx
ERNeuRadDigitizer.cxx
ERNeuRadDigitizerTS.cxx
ERNeuRadDigitizerD.cxx
ERNeuRadPointAnalyser.cxx
ERNeuRadDigiAnalyser.cxx
ERNeuRadContFact.cxx
ERNeuRadDigiPar.cxx
ERNeuRadGeoPar.cxx
ERNeuRadSetup.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERNeuRadLinkDef.h)
Set(LIBRARY_NAME NeuRad)
Set(DEPENDENCIES ERBase ERData)

GENERATE_LIBRARY()

