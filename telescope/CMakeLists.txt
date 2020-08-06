# Create a library called "telescope" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${GEANT4_INCLUDE_DIR}
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
${GEANT4_LIB_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS

geo/ERTelescopeGeoComponentSensetive.cxx
geo/ERTelescopeGeoComponentPassive.cxx
geo/ERTelescopeGeoComponentSingleSi.cxx
geo/ERQTelescopeGeoComponentSingleSi.cxx
geo/ERRTelescopeGeoComponentSingleSi.cxx
geo/ERQTelescopeGeoNonUniformSingleSi.cxx
geo/ERTelescopeGeoComponentDoubleSi.cxx
geo/ERQTelescopeGeoComponentDoubleSi.cxx
geo/ERRTelescopeGeoComponentDoubleSi.cxx
geo/ERQTelescopeGeoComponentCsI.cxx
geo/ERRTelescopeGeoComponentCsI.cxx

ERTelescope.cxx
ERTelescopeDigitizer.cxx
ERTelescopeSetup.cxx
ERTelescopeTrackFinder.cxx
ERTelescopePID.cxx

data/ERTelescopeTrack.cxx
data/ERTelescopeParticle.cxx

)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERTelescopeLinkDef.h)
Set(LIBRARY_NAME telescope)
Set(DEPENDENCIES ERBase ERData Base Core Geom XMLParser G4particles)

GENERATE_LIBRARY()

