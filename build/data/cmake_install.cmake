# Install script for directory: /home/muzalevsky/check/data

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadPoint.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadPhotoElectron.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadPixelSignal.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadDigi.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadStep.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadHit.h"
    "/home/muzalevsky/check/data/NeuRadData/ERNeuRadHitWBT.h"
    "/home/muzalevsky/check/data/muSiData/ERmuSiPoint.h"
    "/home/muzalevsky/check/data/muSiData/ERmuSiHit.h"
    "/home/muzalevsky/check/data/muSiData/ERmuSiTrack.h"
    "/home/muzalevsky/check/data/muSiData/ERmuSiVertex.h"
    "/home/muzalevsky/check/data/GadastData/ERGadastCsIPoint.h"
    "/home/muzalevsky/check/data/GadastData/ERGadastLaBrPoint.h"
    "/home/muzalevsky/check/data/GadastData/ERGadastStep.h"
    "/home/muzalevsky/check/data/GadastData/ERGadastDigi.h"
    "/home/muzalevsky/check/data/ERStack.h"
    "/home/muzalevsky/check/data/ERMCTrack.h"
    "/home/muzalevsky/check/data/ERMCEventHeader.h"
    "/home/muzalevsky/check/data/EREventHeader.h"
    "/home/muzalevsky/check/data/ERLi10EventHeader.h"
    "/home/muzalevsky/check/data/ERLi10MCEventHeader.h"
    "/home/muzalevsky/check/data/NDData/ERNDPoint.h"
    "/home/muzalevsky/check/data/NDData/ERNDHit.h"
    "/home/muzalevsky/check/data/DSRDData/ERDSRDPoint.h"
    "/home/muzalevsky/check/data/DSRDData/ERDSRDHit.h"
    "/home/muzalevsky/check/data/beamtime/RawEvent.h"
    "/home/muzalevsky/check/data/beamtime/AEvent.h"
    "/home/muzalevsky/check/data/beamtime/TGo4EventElement.h"
    "/home/muzalevsky/check/data/beamtime/TLiEvent.h"
    "/home/muzalevsky/check/data/beamtime/cls_RootHit.h"
    "/home/muzalevsky/check/data/beamtime/cls_RootEvent.h"
    "/home/muzalevsky/check/data/beamtime/WCalHit.h"
    "/home/muzalevsky/check/data/beamtime/WCalEvent.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/muzalevsky/check/build/lib/G__ERDataDict_rdict.pcm"
    "/home/muzalevsky/check/build/lib/libERData.rootmap"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so..."
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/muzalevsky/check/build/lib/libERData.so..."
    "/home/muzalevsky/check/build/lib/libERData.so.0"
    "/home/muzalevsky/check/build/lib/libERData.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so..."
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libERData.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_REMOVE
           FILE "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

