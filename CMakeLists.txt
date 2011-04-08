CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Punisher)

# import libraries
IF (NOT TARGET Event)
	ADD_SUBDIRECTORY(externals/Event ${CMAKE_CURRENT_BINARY_DIR}/Event)
ENDIF (NOT TARGET Event)

IF (NOT TARGET Logging)
	ADD_SUBDIRECTORY(externals/Logging ${CMAKE_CURRENT_BINARY_DIR}/Logging)
ENDIF (NOT TARGET Logging)

IF (NOT TARGET Network)
	ADD_SUBDIRECTORY(externals/Network ${CMAKE_CURRENT_BINARY_DIR}/Network)
ENDIF (NOT TARGET Network)

IF (NOT TARGET M)
	ADD_SUBDIRECTORY(externals/M ${CMAKE_CURRENT_BINARY_DIR}/M)
ENDIF (NOT TARGET M)

# libpunisher
ADD_SUBDIRECTORY(libpunisher)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

# includes
INCLUDE_DIRECTORIES(externals/Network/include)
INCLUDE_DIRECTORIES(externals/Logging/include)
INCLUDE_DIRECTORIES(externals/Event/include)
INCLUDE_DIRECTORIES(externals/M/include)
INCLUDE_DIRECTORIES(include)

# apple fixes
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	INCLUDE_DIRECTORIES(/opt/local/include)
	LINK_DIRECTORIES(/opt/local/lib)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# CFLAGS
ADD_DEFINITIONS(-DDEBUG)

INCLUDE(${Punisher_SOURCE_DIR}/libpunisher/SourceFiles.txt)
INCLUDE(${Punisher_SOURCE_DIR}/src/SourceFiles.txt)
INCLUDE(${Punisher_SOURCE_DIR}/test/SourceFiles.txt)

# build executables
ADD_EXECUTABLE(Punisher ${Punisher_SRC} ${Punisher_MAIN})
ADD_EXECUTABLE(statstest ${Punisher_SRC} ${Punisher_STATSTEST})

# link executable to necesary libs
TARGET_LINK_LIBRARIES(Punisher Network Event Logging M yaml pthread crypto)
TARGET_LINK_LIBRARIES(statstest Network Event Logging M yaml pthread crypto)
