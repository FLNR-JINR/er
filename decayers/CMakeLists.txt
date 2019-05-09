# Create a library called "libERDecayers" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/passive
${CMAKE_SOURCE_DIR}/decayers
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERDecayer.cxx
ERDecay.cxx
ERTextDecay.cxx
ERDecay27Fto26O.cxx
ERDecay26Oto24O2n.cxx
ERDecay31Arto30Ar.cxx
ERDecay30Arto28S2p.cxx
ERDecayLi9DetoLi10_Li9n_p.cxx
ERDecayEXP1803.cxx
ERDecayEXP1811.cxx
ERElasticScattering.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERDecayersLinkDef.h)
Set(LIBRARY_NAME ERDecayers)
Set(DEPENDENCIES Base Core)

GENERATE_LIBRARY()