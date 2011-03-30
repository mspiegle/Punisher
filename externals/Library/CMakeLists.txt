CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Library)

ADD_SUBDIRECTORY(M "${CMAKE_CURRENT_BINARY_DIR}/M")
ADD_SUBDIRECTORY(Logging "${CMAKE_CURRENT_BINARY_DIR}/Logging")
ADD_SUBDIRECTORY(Network "${CMAKE_CURRENT_BINARY_DIR}/Network")
ADD_SUBDIRECTORY(Event "${CMAKE_CURRENT_BINARY_DIR}/Event")

#FILE(GLOB M_SRC ${M_SOURCE_DIR}/src/*)
#FILE(GLOB Logging_SRC ${Logging_SOURCE_DIR}/src/*)
#FILE(GLOB Network_SRC ${Network_SOURCE_DIR}/src/*)
INCLUDE(${Network_SOURCE_DIR}/src/SourceFiles.txt)
INCLUDE(${Logging_SOURCE_DIR}/src/SourceFiles.txt)
INCLUDE(${M_SOURCE_DIR}/src/SourceFiles.txt)
INCLUDE(${Event_SOURCE_DIR}/src/SourceFiles.txt)

#FILE(GLOB Event_SRC ${Event_SOURCE_DIR}/src/*)
#FILE(GLOB Event_SRC ${Event_SOURCE_DIR}/src/Item.cxx
#	${Event_SOURCE_DIR}/src/EpollManager.cxx
#	${Event_SOURCE_DIR}/src/Manager.cxx)

#MESSAGE(STATUS "Network_SRC: " ${Network_SRC})
#MESSAGE(STATUS "M_SRC: " ${M_SRC})
#MESSAGE(STATUS "Logging_SRC: " ${Logging_SRC})
#MESSAGE(STATUS "Event_SRC: " ${Event_SRC})

INCLUDE_DIRECTORIES(${M_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Network_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Logging_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Event_SOURCE_DIR}/include)

ADD_LIBRARY(Library ${M_SRC} ${Logging_SRC} ${Network_SRC} ${Event_SRC})
TARGET_LINK_LIBRARIES(Library M Logging Network Event)
