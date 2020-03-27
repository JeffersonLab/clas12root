cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(clas12hipo4root)

	
set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_INSTALL_MANDIR ${CMAKE_CURRENT_SOURCE_DIR}/man)
set(CMAKE_CXX_FLAGS "-fPIC -O3")


##########For local ROOT cmake files comment in the line below and remove the lines commented ##USEROOTSYS
#####include("cmake/FindROOT.cmake")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) ############USEROOTSYS

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Proof)  ###########USEROOTSYS

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE}) ##########USEROOTSYS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

#hipo expects lz4 includes to be in lib...
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#set(ADD_COMPILER_FLAGS_PREPEND "-O3")
set(CMAKE_C_FLAGS "-O3 ${CMAKE_C_FLAGS}")
set(LZ4_BUNDLED_MODE OFF)
set(BUILD_SHARED_LIBS ON)
set(BUILD_STATIC_LIBS OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lz4/contrib/cmake_unofficial)
set(LZ4_LIBRARY lz4)
set(LZ4_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib)
include_directories(${LZ4_INCLUDE_DIR})
add_definitions(-D__LZ4__)

#after Lz4 change include to include
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#include rcdb c++ header library
IF (DEFINED ENV{RCDB_HOME})
  include_directories($ENV{RCDB_HOME}/cpp/include)
ENDIF (DEFINED ENV{RCDB_HOME})

#find path to mysql include directory
FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
  /usr/local/include/mysql
  /usr/include/mysql
  )

#find path to mysql library
SET(MYSQL_NAMES mysqlclient mysqlclient_r)
FIND_LIBRARY(MYSQL_LIBRARY
  NAMES ${MYSQL_NAMES}
  PATHS /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64
  PATH_SUFFIXES mysql
  )

#include mysql library and directory if they were found
IF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  include_directories(${MYSQL_INCLUDE_DIR})
  link_libraries(${MYSQL_LIBRARY})
  add_definitions(-DRCDB_MYSQL)
ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hipo4)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/hipo4)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root)




