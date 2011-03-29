CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Punisher)

ADD_SUBDIRECTORY(Library "${CMAKE_CURRENT_BINARY_DIR}/Library")

FILE(GLOB Punisher_SRC ${Punisher_SOURCE_DIR}/src/*)

INCLUDE_DIRECTORIES(${Library_SOURCE_DIR}/Network/include)
INCLUDE_DIRECTORIES(${Library_SOURCE_DIR}/Logging/include)
INCLUDE_DIRECTORIES(${Library_SOURCE_DIR}/Event/include)
INCLUDE_DIRECTORIES(${Library_SOURCE_DIR}/M/include)

ADD_LIBRARY(Library ${M_SRC} ${Logging_SRC} ${Network_SRC} ${Event_SRC})
TARGET_LINK_LIBRARIES(Library M Logging Network Event)
