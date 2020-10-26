# Create a library called "libBeamdet" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${GEANT4_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/BeamDet/data/
${CMAKE_SOURCE_DIR}/BeamDet/
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/data/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
${GEANT4_LIB_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
    data/ERBeamDetTOFPoint.cxx
    data/ERBeamDetMWPCPoint.cxx
    data/ERBeamDetTargetPoint.cxx
	data/ERBeamDetTrack.cxx
	data/ERBeamDetParticle.cxx

	ERBeamDet.cxx
	ERBeamDetDigitizer.cxx
	ERBeamDetTrackFinder.cxx
	ERBeamDetSetup.cxx
	ERBeamDetPID.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBeamDetLinkDef.h)
Set(LIBRARY_NAME ERBeamDet)
Set(DEPENDENCIES ERBase ERData Base Core Geom XMLParser G4particles)

GENERATE_LIBRARY()