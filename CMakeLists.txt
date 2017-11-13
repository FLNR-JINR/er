# The name of our project is �EXPERTROOT�. CMakeLists files in this project can
# refer to the root source directory of the project as ${EXPERTROOT_SOURCE_DIR} or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${EXPERTROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.

# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1 FATAL_ERROR)

# Set name of our project to "R3BROOT". Has to be done
# after check of cmake version
project(EXPERTROOT)

# Check if the FairRoot is still in source, since Jan 2014 FairRoot
# should be installed separately

FIND_PATH(FAIRBASE NAMES FairRun.h PATHS
  ${CMAKE_SOURCE_DIR}/base/steer
  NO_DEFAULT_PATH
)

IF (FAIRBASE)
  Message(STATUS "Found FAIRBASE")
  SET(FAIRBASE ${FAIRBASE})
ELSE (FAIRBASE)
  Message(STATUS "NOT Found FAIRBASE")
  IF (NOT DEFINED ENV{FAIRROOTPATH})
     MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
  ENDIF (NOT DEFINED ENV{FAIRROOTPATH})
  SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
  SET(FAIRROOTPATH_BUILD $ENV{FAIRROOTPATH_BUILD})
ENDIF (FAIRBASE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})

# Check if the compiler support specific C++11 features
# Up to now this is only a check since the code does not use
# any of the features of the new standard

IF (FAIRROOTPATH)
  Set(CheckSrcDir "${FAIRROOTPATH}/share/fairbase/cmake/checks")
ELSE (FAIRROOTPATH)
  Set(CheckSrcDir "${CMAKE_SOURCE_DIR}/cmake/checks")
ENDIF (FAIRROOTPATH)

IF (FAIRROOTPATH)
  find_package(FairRoot)
ENDIF (FAIRROOTPATH)

include(CheckCXX11Features)


# Load some basic macros which are needed later on
include(FairMacros)
include(WriteConfigFile)
include(Dart)
include(CheckCompiler)


#Check the compiler and set the compile and link flags
Check_Compiler()

Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")
Set(VMCWORKDIR ${CMAKE_SOURCE_DIR})

Option(USE_PATH_INFO "Information from PATH and LD_LIBRARY_PATH are
used." OFF)
IF (USE_PATH_INFO)
  Set(PATH $ENV{PATH})
  IF (APPLE)
    Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
  ELSE (APPLE)
    Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
  ENDIF (APPLE)
ENDIF (USE_PATH_INFO)

# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if we are on an UNIX system. If not stop with an error
# message
IF (NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF (NOT UNIX)

#Check if necessary environment variables are set
#If not stop execution
IF (NOT EXPERTROOT_MINIMAL)
  IF (NOT DEFINED ENV{SIMPATH})
    MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
  ENDIF (NOT DEFINED ENV{SIMPATH})
  STRING(REGEX MATCHALL "[^:]+" PATH $ENV{PATH})
  SET(SIMPATH $ENV{SIMPATH})
ELSE (NOT EXPERTROOT_MINIMAL)
  IF (NOT DEFINED ENV{ROOTSYS})
    MESSAGE(FATAL_ERROR "You did not define the environment variable ROOTSYS which is nedded for the installation of the minimal version of FairRoot. Please set this variable and execute cmake again.")
  ENDIF (NOT DEFINED ENV{ROOTSYS})
ENDIF (NOT EXPERTROOT_MINIMAL)

# Check if the external packages are installed into a separate install
# directory
CHECK_EXTERNAL_PACKAGE_INSTALL_DIR()

IF (FAIRSOFT_EXTERN)
  set(BOOST_ROOT ${SIMPATH})
  set(GSL_DIR ${SIMPATH})
ELSE (FAIRSOFT_EXTERN)
  set(BOOST_ROOT ${SIMPATH}/basics/boost)
  set(GSL_DIR ${SIMPATH}/basics/gsl)
ENDIF (FAIRSOFT_EXTERN)

IF (FAIRROOT_FOUND)
  Set(Boost_NO_SYSTEM_PATHS TRUE)
  Set(Boost_NO_BOOST_CMAKE TRUE)
  Message("-- Looking for Boost ...")
  # If an older version of boost is found both of the variables below are
  # cached and in a second cmake run, a good boost version is found even
  # if the version is to old.
  # To overcome this problem both variables are cleared before checking
  # for boost.
  Unset(Boost_INCLUDE_DIR CACHE)
  Unset(Boost_LIBRARY_DIRS CACHE)
  find_package(Boost 1.41 COMPONENTS thread system timer)
  IF (Boost_FOUND)
    Set(Boost_Avail 1)
  ELSE (Boost_FOUND)
    Set(Boost_Avail 0)
  ENDIF (Boost_FOUND)
ENDIF (FAIRROOT_FOUND)

SetBasicVariables()
# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory
IF (EXPERTROOT_MINIMAL)
  MESSAGE("******* Install minimal FairRoot Version ********")
  find_package(ROOT 5.32.00 REQUIRED)
ELSE (EXPERTROOT_MINIMAL)
  find_package(ROOT 5.32.00 REQUIRED)
  #find_package(PLUTO REQUIRED)
  find_package(GENERATORS REQUIRED)
  find_package(GEANT3 REQUIRED)
  find_package(GEANT4 REQUIRED)
  find_package(GEANT4DATA)
  find_package(GEANT4VMC REQUIRED)
  find_package(CLHEP REQUIRED)
  find_package(RuleChecker)
ENDIF (EXPERTROOT_MINIMAL)

MESSAGE(STATUS "GEANT4 = " ${GEANT4_INCLUDE_DIR})

# set a variable which should be used in all CMakeLists.txt
# Defines all basic include directories from fairbase
SetBasicVariables()

# Set the library version in the main CMakeLists.txt
SET(EXPERTROOT_MAJOR_VERSION 0)
SET(EXPERTROOT_MINOR_VERSION 0)
SET(EXPERTROOT_PATCH_VERSION 0)
SET(EXPERTROOT_VERSION "${FAIRROOT_MAJOR_VERSION}.${FAIRROOT_MINOR_VERSION}.${FAIRROOT_PATCH_VERSION}")
SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
    VERSION "${EXPERTROOT_VERSION}"
    SOVERSION "${EXPERTROOT_MAJOR_VERSION}"
    SUFFIX ".so"
)

SET(EXPERTLIBDIR ${CMAKE_BINARY_DIR}/lib)
SET(LD_LIBRARY_PATH ${EXPERTLIBDIR} ${LD_LIBRARY_PATH})


Generate_Version_Info()

# Recurse into the given subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.


IF (NOT FAIRROOT_FOUND)
add_subdirectory (fairtools)
add_subdirectory (base)
add_subdirectory (dbase)
add_subdirectory (geobase)
add_subdirectory (parbase)
add_subdirectory (generators)
add_subdirectory (geane)
add_subdirectory (trackbase)
add_subdirectory (MbsAPI)
ENDIF (NOT FAIRROOT_FOUND)

add_subdirectory (base)
add_subdirectory (data)
add_subdirectory (decayers)
add_subdirectory (passive)
add_subdirectory (NeuRad)
add_subdirectory (muSi)
add_subdirectory (Gadast)
add_subdirectory (generators)
add_subdirectory (ND)
add_subdirectory (telescope)
add_subdirectory (global)
add_subdirectory (beamtime)
add_subdirectory (BeamDet)
add_subdirectory (field)
#add_subdirectory (SensPlane)

Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-opengl
                OUTPUT_VARIABLE EveIsBuild
                OUTPUT_STRIP_TRAILING_WHITESPACE
               )

IF (${EveIsBuild} STREQUAL yes)
  Message("Build the eventdisplay.")
IF (NOT FAIRROOT_FOUND)
  add_subdirectory (eventdisplay)
ENDIF (NOT FAIRROOT_FOUND)
ELSE (${EveIsBuild} STREQUAL yes)
  Message("The eventdisplay is not build, there is no OpenGL support.")
ENDIF (${EveIsBuild} STREQUAL yes)

Option(BUILD_DOXYGEN "Build Doxygen" OFF)
IF (BUILD_DOXYGEN)
  MESSAGE(STATUS "*** Building the Doxygen documentaion ***")
  ADD_SUBDIRECTORY(doxygen)
ENDIF (BUILD_DOXYGEN)


IF (RULE_CHECKER_FOUND)

  ADD_CUSTOM_TARGET(RULES
     COMMAND ${RULE_CHECKER_SCRIPT1} ${CMAKE_BINARY_DIR} viol > violations.html
     DEPENDS $ENV{ALL_RULES})

ENDIF (RULE_CHECKER_FOUND)

MESSAGE (${ROOT_INCLUDE_DIR})

IF (NOT EXPERTROOT_MINIMAL)
  WRITE_CONFIG_FILE(config.sh)
  WRITE_CONFIG_FILE(config.csh)
ELSE (NOT EXPERTROOT_MINIMAL)
  WRITE_MINIMAL_CONFIG_FILE(config.sh)
ENDIF (NOT EXPERTROOT_MINIMAL)
