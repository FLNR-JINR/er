# Create a library called "libERBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${GEANT4VMC_INCLUDE_DIR}
${GEANT4_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/passive
${CMAKE_SOURCE_DIR}/decayers
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${GEANT4_LIB_DIRS}
${FAIRROOT_LIBRARY_DIR}
${GEANT4VMC_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
ERDetector.cxx
ERMCApplication.cxx
ERRecoMCApplication.cxx
ERRunSim.cxx
ERSupport.cxx
ERDetectorContFact.cxx
ERDetectorGeoPar.cxx
ERDigitizer.cxx
ERRunAna.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERBaseLinkDef.h)
Set(LIBRARY_NAME ERBase)
Set(DEPENDENCIES Base Core ERDecayers G4processes G4run G4physicslists geant4vmc)

GENERATE_LIBRARY()

