# Create a library called "libERBeamTime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/beamtime/
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/data/
${CMAKE_SOURCE_DIR}/data/beamtime/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	ERDRS4Source.cxx
	ERTektronixSource.cxx
	ERRawToAnalyzeConverter.cxx

	ERLiCalibrator.cxx
	ERRootSource.cxx
	
	ERNXyterTreeSource.cxx
	ERNXyterCalibrator.cxx
	ERpmtPixelMap.cxx
	ERSimtoRaw.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBeamtimeLinkDef.h)
Set(LIBRARY_NAME ERBeamTime)
Set(DEPENDENCIES ERBase ERData Base Core)

GENERATE_LIBRARY()

