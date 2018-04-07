# Create a library called "telescope" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base/
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/telescope/
${CMAKE_SOURCE_DIR}/BeamDet/data
${CMAKE_SOURCE_DIR}/telescope/data
${CMAKE_SOURCE_DIR}/telescope/geo
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS

#simulation
ERRTelescope.cxx
#ERRTelescopeDigitizer.cxx
ERRTelescopeHitFinder.cxx
ERRTelescopeSetup.cxx

data/ERRTelescopeCsIPoint.cxx
data/ERRTelescopeSiPoint.cxx
data/ERRTelescopeCsIDigi.cxx
data/ERRTelescopeSiDigi.cxx
data/ERRTelescopeHit.cxx

geo/ERQTelescopeGeoComponentDoubleSi.cxx
geo/ERQTelescopeGeoComponentSingleSi.cxx
geo/ERQTelescopeGeoComponentCsI.cxx

ERQTelescope.cxx
ERQTelescopeDigitizer.cxx
ERQTelescopeSetup.cxx
ERQTelescopeTrackFinder.cxx

data/ERQTelescopeSiPoint.cxx
data/ERQTelescopeSiDigi.cxx
data/ERQTelescopeCsIPoint.cxx
data/ERQTelescopeCsIDigi.cxx
data/ERQTelescopeTrack.cxx

#AculRaw
data/AculRaw.cxx


#beamtest
ERQTelescopeCalibrator.cxx
ERRTelescopeCalibrator.cxx
#ERTelescopeReconstructorNew.cxx

data/ERRTelescopeEvent.cxx
data/ERRTelescopeCalEvent.cxx
data/ERQTelescopeEvent.cxx
data/ERQTelescopeCalEvent.cxx
data/ERTelescopeRecoEvent.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERTelescopeLinkDef.h)
Set(LIBRARY_NAME telescope)
Set(DEPENDENCIES ERBase ERData Base Core Geom XMLParser)

GENERATE_LIBRARY()

