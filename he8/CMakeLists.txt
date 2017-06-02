# Create a library called "libhe8lib" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/he8/
${CMAKE_SOURCE_DIR}/he8/data
${CMAKE_SOURCE_DIR}/he8/beamdet/
${CMAKE_SOURCE_DIR}/he8/beamdet/data
${CMAKE_SOURCE_DIR}/he8/telescope
${CMAKE_SOURCE_DIR}/he8/telescope/data
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	beamdet/data/ERBeamDetEvent.cxx
	beamdet/data/ERBeamDetCalEvent.cxx
	beamdet/data/ERBeamDetRecoEvent.cxx
	beamdet/ERBeamDetCalibrator.cxx
	beamdet/ERBeamDetReconstructor.cxx
	telescope/data/ERRTelescopeEvent.cxx
	telescope/data/ERRTelescopeCalEvent.cxx
	telescope/data/ERQTelescopeEvent.cxx
	telescope/data/ERQTelescopeCalEvent.cxx
	telescope/data/ERTelescopeRecoEvent.cxx
	telescope/ERQTelescopeCalibrator.cxx
	telescope/ERRTelescopeCalibrator.cxx
	telescope/ERTelescopeReconstructor.cxx
	data/ERHe8EventHeader.cxx
	data/ERRawEvent.cxx
	data/ERTelData.cxx
	data/ERInclusiveData.cxx
	data/ERParticle.cxx
	ERSupport.cxx
	#ERHe8Analyzer.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERHe8LinkDef.h)
Set(LIBRARY_NAME he8)
Set(DEPENDENCIES ERBase ERData Base Core)

GENERATE_LIBRARY()

