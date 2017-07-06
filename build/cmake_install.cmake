# Install script for directory: /home/muzalevsky/check

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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/muzalevsky/check/build/base/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/data/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/decayers/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/passive/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/NeuRad/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/muSi/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/Gadast/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/generators/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/ND/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/DSRD/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/global/cmake_install.cmake")
  INCLUDE("/home/muzalevsky/check/build/beamtime/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/muzalevsky/check/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/muzalevsky/check/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
