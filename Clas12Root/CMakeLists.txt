set(CLAS12ROOT Clas12Root)
set(CLAS12BANKS Clas12Banks)
#set(HIPO4 hipo4)

#include rcdb c++ header library
IF (DEFINED ENV{RCDB_HOME})
  include_directories($ENV{RCDB_HOME}/cpp/include)
  add_definitions(-DCLAS_RCDB)
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

ENDIF (DEFINED ENV{RCDB_HOME})

IF (DEFINED ENV{CCDB_HOME})
  include_directories($ENV{CCDB_HOME}/include)
ENDIF (DEFINED ENV{CCDB_HOME})


include_directories(${CLAS12BANKS_DIR})
include_directories(${HIPO_INCLUDE_DIR})

#include_directories(${hipo4_INCLUDE_DIRS})#in case change to pkg, this is needed to generate ROOT dict

ROOT_GENERATE_DICTIONARY(G__${CLAS12ROOT}  BankHist.h HipoHist.h HipoTreeMaker.h HipoChain.h HipoChainWriter.h HipoRootAction.h   HipoROOTOut.h ParticleHist.h HipoSelector.h ParticleTree.h LINKDEF Clas12RootLinkDef.h)

add_library(${CLAS12ROOT} SHARED BankHist.cpp HipoHist.cpp HipoTreeMaker.cpp HipoChain.cpp HipoChainWriter.cpp HipoRootAction.cpp   HipoROOTOut.cpp ParticleHist.cpp  HipoSelector.cpp ParticleTree.cpp G__${CLAS12ROOT}.cxx)

#target_link_libraries(${CLAS12ROOT} PUBLIC ${ROOT_LIBRARIES} ${CLAS12BANKS} PkgConfig::${HIPO4})#in case change to pkg
#target_include_directories(${CLAS12ROOT} SYSTEM PUBLIC PkgConfig::${HIPO4})#in case change to pkg

target_link_libraries(${CLAS12ROOT} PUBLIC ${ROOT_LIBRARIES} ${CLAS12BANKS} ${HIPO_LIBS})


install(TARGETS ${CLAS12ROOT} 
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")


install (FILES  ${CMAKE_CURRENT_BINARY_DIR}/libClas12Root_rdict.pcm    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)
