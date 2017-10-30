# Create a library called "libNeuRad" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/data/
${CMAKE_SOURCE_DIR}/NeuRad/
${CMAKE_SOURCE_DIR}/NeuRad/data
${CMAKE_SOURCE_DIR}/base/
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}

) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	ERNeuRad.cxx
	ERNeuRadDigitizer.cxx
	ERNeuRadContFact.cxx
	ERNeuRadDigiPar.cxx
	ERNeuRadGeoPar.cxx
	ERNeuRadSetup.cxx
	ERNeuRadHitFinder.cxx
	ERNeuRadHitFinderMF.cxx
	ERNeuRadHitFinderWBT.cxx
	ERNeuRadMatcher.cxx
	ERNeuRadViewer.cxx
	data/ERNeuRadPoint.cxx
	data/ERNeuRadPhotoElectron.cxx
	data/ERNeuRadPixelSignal.cxx
	data/ERNeuRadDigi.cxx
	data/ERNeuRadStep.cxx
	data/ERNeuRadHit.cxx
	data/ERNeuRadHitWBT.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERNeuRadLinkDef.h)
Set(LIBRARY_NAME NeuRad)
Set(DEPENDENCIES ERBase ERData Base Core Geom)

GENERATE_LIBRARY()
