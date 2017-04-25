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
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	data/ERHe8EventHeader.cxx
	data/ERRawEvent.cxx
	data/ERTelescopeEvent.cxx
	data/ERMwpcEvent.cxx
	data/ERDsrdEvent.cxx
	data/ERTofEvent.cxx
	data/ERDsrdCalEvent.cxx
	data/ERTelescopeCalEvent.cxx
	data/ERTofCalEvent.cxx
	ERDsrdCalibrator.cxx
	ERTelescopeCalibrator.cxx
	ERTofCalibrator.cxx
	ERTracker.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERHe8LinkDef.h)
Set(LIBRARY_NAME he8)
Set(DEPENDENCIES ERBase ERData Base Core)

GENERATE_LIBRARY()

