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

#find_package(hipo4)
find_package(PkgConfig REQUIRED)
pkg_check_modules(hipo4 REQUIRED IMPORTED_TARGET hipo4)

#after Lz4 change include to include
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#include clas12-qadb c++ library and rapidjson library
IF (DEFINED ENV{QADB})
  include_directories($ENV{QADB}/srcC/include)
  include_directories($ENV{QADB}/srcC/rapidjson/include)
  add_definitions(-DCLAS_QADB)
ENDIF (DEFINED ENV{QADB})



set(CLAS12ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root)
#set(CLAS12HIPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hipo4)
set(CLAS12BANKS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks)

#include_directories(${CLAS12HIPO_DIR})
#add_subdirectory (${CLAS12HIPO_DIR})

include_directories(${CLAS12BANKS_DIR})
add_subdirectory (${CLAS12BANKS_DIR})

include_directories(${CLAS12ROOT_DIR})
add_subdirectory (${CLAS12ROOT_DIR})

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hipo4)
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/hipo4)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks)
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Clas12Banks)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root)
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root)


