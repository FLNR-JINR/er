# The name of our project is “EXPERTROOT”.  CMakeLists files in this project can
# refer to the root source directory of the project as ${EXPERTROOT_SOURCE_DIR} or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as 
# ${EXPERTROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.

# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1 FATAL_ERROR)

# Set name of our project to "EXPERTROOT". Has to be done
# after check of cmake version                        
project(EXPERTROOT)

#If (FAIRBASE)
#  Message(STATUS "Found FAIRBASE")
#  SET(FAIRBASE ${FAIRBASE})
#Else (FAIRBASE)
#  Message(STATUS "NOT Found FAIRBASE")
#  IF(NOT DEFINED ENV{FAIRROOTPATH})
#     MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
#  ENDIF(NOT DEFINED ENV{FAIRROOTPATH})
#  SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
#EndIf (FAIRBASE)

#check environments variables
IF(NOT DEFINED ENV{FAIRROOTPATH})
  MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot sources. Please set this variable and execute cmake again.")
ELSE(NOT DEFINED ENV{FAIRROOTPATH})
  SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
  MESSAGE( "FAIRROOTPATH = " ${FAIRROOTPATH})
ENDIF(NOT DEFINED ENV{FAIRROOTPATH})
  
IF(NOT DEFINED ENV{FAIRROOT_BINARY_DIR})
  MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOT_BINARY_DIR which is needed to find FairRoot binaries. Please set this variable and execute cmake again.")
ELSE (NOT DEFINED ENV{FAIRROOT_BINARY_DIR})
  SET(FAIRROOT_BINARY_DIR $ENV{FAIRROOT_BINARY_DIR}) 
  MESSAGE( "FAIRROOT_BINARY_DIR = " ${FAIRROOT_BINARY_DIR})
ENDIF (NOT DEFINED ENV{FAIRROOT_BINARY_DIR})

IF(NOT DEFINED ENV{SIMPATH})
     MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is needed to find External packages. Please set this variable and execute cmake again.")    
ELSE (NOT DEFINED ENV{SIMPATH})
  SET(SIMPATH $ENV{SIMPATH})
  MESSAGE( "SIMPATH = " ${SIMPATH})
ENDIF (NOT DEFINED ENV{SIMPATH})

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ 
# is checked

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Check if the compiler support specific C++11 features
# Up to now this is only a check since the code does not use
# any of the features of the new standard

#Set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/cmake/checks"  ${CMAKE_MODULE_PATH})
Set(CheckSrcDir "${FAIRROOTPATH}/cmake/checks")
#find_package(FairRoot)

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
used."OFF)
If(USE_PATH_INFO)
  Set(PATH $ENV{PATH})
  If (APPLE)
    Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
  Else (APPLE)
    Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
  EndIf (APPLE)
EndIf(USE_PATH_INFO)

# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if we are on an UNIX system. If not stop with an error
# message
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)  

# Check if the external packages are installed into a separate install
# directory
CHECK_EXTERNAL_PACKAGE_INSTALL_DIR()

SetBasicVariables()

# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory
find_package(ROOT 5.32.00 REQUIRED)
find_package(PLUTO REQUIRED)
find_package(GENERATORS REQUIRED)
find_package(GEANT3 REQUIRED)
find_package(GEANT4 REQUIRED)
find_package(GEANT4DATA)
find_package(GEANT4VMC REQUIRED)
find_package(CLHEP REQUIRED)
find_package(RuleChecker)

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
SET(LD_LIBRARY_PATH  ${EXPERTLIBDIR} ${LD_LIBRARY_PATH})


Generate_Version_Info()

# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.

#add_subdirectory (NeuRad)
#add_subdirectory (ExpertData)
#add_subdirectory (passive)

Option(BUILD_DOXYGEN "Build Doxygen" OFF)
if(BUILD_DOXYGEN)
  MESSAGE(STATUS "*** Building the Doxygen documentaion ***")
  ADD_SUBDIRECTORY(doxygen)
endif(BUILD_DOXYGEN)


if(RULE_CHECKER_FOUND)

  ADD_CUSTOM_TARGET(RULES 
     COMMAND ${RULE_CHECKER_SCRIPT1} ${CMAKE_BINARY_DIR} viol > violations.html
     DEPENDS $ENV{ALL_RULES})

endif(RULE_CHECKER_FOUND)

WRITE_CONFIG_FILE(config.sh)