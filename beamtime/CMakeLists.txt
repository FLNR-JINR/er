# Create a library called "libERBeamTime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/data/
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/beamtime/
${CMAKE_SOURCE_DIR}/NeuRad/data/
${CMAKE_SOURCE_DIR}/data/data/
${CMAKE_SOURCE_DIR}/beamtime/data/
)

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

set(SRCS
	ERDRS4Source.cxx
	ERTektronixSource.cxx
	ERRawToAnalyzeConverter.cxx

	ERLiCalibrator.cxx
	ERRootSource.cxx
	ERRootSourceOld.cxx
	ERNXyterTreeSource.cxx
	ERNXyterCalibrator.cxx
	ERpmtPixelMap.cxx
	ERSimtoRaw.cxx

	data/ERNeuRadRawEvent.cxx
	data/ERNeuRadAEvent.cxx

	data/TGo4EventElement.cxx
	data/TLiEvent.cxx

	data/cls_RootHit.cxx
	data/cls_RootEvent.cxx

	data/WCalHit.cxx
	data/WCalEvent.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBeamtimeLinkDef.h)
Set(LIBRARY_NAME ERBeamTime)
Set(DEPENDENCIES ERBase ERData Base Core)

if (ACCULINNA_GO4)
	add_definitions( -DACCULINA_GO4)
	list(APPEND SRCS ERDigibuilder.cxx)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/)
	message(STATUS ${ACCULINNA_GO4}/lib/)
	list(APPEND LINK_DIRECTORIES ${ACCULINNA_GO4}/lib/)
	list(APPEND DEPENDENCIES Data)
endif()

include_directories( ${INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

GENERATE_LIBRARY()

