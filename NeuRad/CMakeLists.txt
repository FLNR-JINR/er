# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
#put here all directories where header files are located
${BASE_INCLUDE_DIRECTORIES}
${R3BROOT_SOURCE_DIR}/passive
${R3BROOT_SOURCE_DIR}/mcstack
${R3BROOT_SOURCE_DIR}/NeuRad
${R3BROOT_SOURCE_DIR}/ExpertData/NeuRadData/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ExpertNeuRad.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ExpertNeuRadLinkDef.h)
Set(LIBRARY_NAME NeuRad)
Set(DEPENDENCIES )

GENERATE_LIBRARY()

