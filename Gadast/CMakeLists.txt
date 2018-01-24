# Create a library called "libERGadast" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/data/
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/Gadast/
${CMAKE_SOURCE_DIR}/Gadast/data
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERGadast.cxx
ERGadastDigiPar.cxx
ERGadastGeoPar.cxx
ERGadastContFact.cxx
ERGadastSetup.cxx
ERGadastDigitizer.cxx
data/ERGadastCsIPoint.cxx
data/ERGadastLaBrPoint.cxx
data/ERGadastStep.cxx
data/ERGadastCsIDigi.cxx
data/ERGadastLaBrDigi.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERGadastLinkDef.h)
Set(LIBRARY_NAME ERGadast)
Set(DEPENDENCIES ERData ERBase Base Core)

GENERATE_LIBRARY()

