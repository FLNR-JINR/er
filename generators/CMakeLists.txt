# Create a library called "libgenerators" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${GEANT4_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/generators
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
${GEANT4_LIB_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	ERGammaGenerator.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERGeneratorsLinkDef.h)
Set(LIBRARY_NAME generators)
Set(DEPENDENCIES ERBase ERData)

GENERATE_LIBRARY()

