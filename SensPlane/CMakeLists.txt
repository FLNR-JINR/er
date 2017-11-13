# Create a library called "libSensPlane" which includes the source files given in
# the array.
# The extension is already found. Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${CMAKE_SOURCE_DIR}/SensPlane/
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
) 

Link_Directories(${LINK_DIRECTORIES})

Set(SRCS
  ERSensPlane.cxx
  ERSensPlanePoint.cxx
)

Set(HEADERS )
Set(LINKDEF ERSensPlaneLinkDef.h)
Set(LIBRARY_NAME SensPlane)
Set(DEPENDENCIES Base)

GENERATE_LIBRARY()
