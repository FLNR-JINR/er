# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/telescope/
${CMAKE_SOURCE_DIR}/telescope/data
${CMAKE_SOURCE_DIR}/BeamDet/data
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERRTelescope.cxx
ERRTelescopeHitFinder.cxx
ERRTelescopeSetup.cxx
ERQTelescope.cxx
data/ERRTelescopePoint.cxx
data/ERRTelescopeHit.cxx
data/ERQTelescopeSiPoint.cxx
data/ERRTelescopeEvent.cxx
data/ERRTelescopeCalEvent.cxx
data/ERQTelescopeEvent.cxx
data/ERQTelescopeCalEvent.cxx
data/ERTelescopeRecoEvent.cxx
data/ERQTelescopeSiDigi.cxx
ERQTelescopeCalibrator.cxx
ERRTelescopeCalibrator.cxx
ERTelescopeReconstructor.cxx
ERQTelescopeDigitizer.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERRTelescopeLinkDef.h)
Set(LIBRARY_NAME telescope)
Set(DEPENDENCIES ERBase ERData Base Core)

GENERATE_LIBRARY()

