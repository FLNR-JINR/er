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
${CMAKE_SOURCE_DIR}/BeamDet/data/
)

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

set(SRCS
	ERDRS4Source.cxx
	ERTektronixSource.cxx
	ERRawToAnalyzeConverter.cxx

	#ERLiCalibrator.cxx
	ERRootSource.cxx
	ERRootSourceOld.cxx
	ERNXyterTreeSource.cxx
	ERNXyterCalibrator.cxx
	ERpmtPixelMap.cxx
	ERSimtoRaw.cxx

	data/ERNeuRadRawEvent.cxx
	data/ERNeuRadAEvent.cxx

	#data/TLiEvent.cxx

	data/cls_RootHit.cxx
	data/cls_RootEvent.cxx

	data/WCalHit.cxx
	data/WCalEvent.cxx
)

Set(LINKDEF ERBeamtimeLinkDef.h)
Set(LIBRARY_NAME ERBeamTime)
Set(DEPENDENCIES ERBase ERData Base Core)

if (ACCULINNA_GO4)
	add_definitions( -DACCULINA_GO4)
	list(APPEND SRCS ERDigibuilder.cxx)
	list(APPEND SRCS unpack/ERUnpack.cxx)
	list(APPEND SRCS unpack/ERBeamDetUnpack.cxx)
	list(APPEND SRCS unpack/ERTelescopeUnpack.cxx)
	list(APPEND INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/beamtime/unpack/)
	list(APPEND INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/telescope/data/)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/reader)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/data)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/go4pieces)
	list(APPEND INCLUDE_DIRECTORIES ${ACCULINNA_GO4}/include/setupconfigcppwrapper)
	message(STATUS ${ACCULINNA_GO4}/lib/)
	list(APPEND LINK_DIRECTORIES ${ACCULINNA_GO4}/lib/)
	list(APPEND DEPENDENCIES AcGo4Data)
	list(APPEND DEPENDENCIES AcGo4Reader)
endif()

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")



include_directories( ${INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})

GENERATE_LIBRARY()

