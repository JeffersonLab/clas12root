


set(CLASS_LIST_CPP helflip.cpp helonline.cpp vertdoca.cpp runconfig.cpp event.cpp ftbevent.cpp particle.cpp ftbparticle.cpp mcparticle.cpp mcevent.cpp  mcmatch.cpp scaler.cpp vtp.cpp particle_detector.cpp scintillator.cpp  scintextras.cpp tracker.cpp traj.cpp forwardtagger.cpp cherenkov.cpp calorimeter.cpp calextras.cpp covmatrix.cpp region_particle.cpp region_ft.cpp region_fdet.cpp region_cdet.cpp region_band.cpp clas12writer.cpp clas12reader.cpp mesonex_trigger.cpp scaler_reader.cpp clas12databases.cpp qadb_reader.cpp ccdb_reader.cpp rcdb_reader.cpp)

set(CLASS_LIST_H helflip.h  helonline.h vertdoca.h runconfig.h event.h ftbevent.h particle.h ftbparticle.h mcparticle.h  mcevent.h mcmatch.h scaler.h vtp.h particle_detector.h scintillator.h scintextras.h tracker.h traj.h forwardtagger.h cherenkov.h calorimeter.h calextras.h covmatrix.h region_particle.h region_ft.h region_fdet.h region_cdet.h region_band.h clas12writer.h clas12reader.h mesonex_trigger.h  scaler_reader.h clas12databases.h  qadb_reader.h ccdb_reader.h rcdb_reader.h)

IF (DEFINED ENV{QADB})
  set(CLASS_LIST_CPP ${CLASS_LIST_CPP} jsonFileMerger.cpp)
  set(CLASS_LIST_H ${CLASS_LIST_H} jsonFileMerger.h)
  add_definitions(-DCLAS_QADB)
ENDIF (DEFINED ENV{QADB})

#include rcdb c++ header library
IF (DEFINED ENV{CCDB_HOME})
  # CCDB related
  include_directories($ENV{CCDB_HOME}/include)
  link_libraries($ENV{CCDB_HOME}/lib/libccdb${CMAKE_SHARED_LIBRARY_SUFFIX})
  add_definitions(-DCLAS_CCDB)
ENDIF (DEFINED ENV{CCDB_HOME})

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
  #IF (MYSQL_INCLUDE_DIR)
    include_directories(${MYSQL_INCLUDE_DIR})
    link_libraries(${MYSQL_LIBRARY})
    add_definitions(-DRCDB_MYSQL)
  ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
 #ENDIF (MYSQL_INCLUDE_DIR)

 
 #This should allow RCDB lite but I get an error when it tries to open file
 #   terminate called after throwing an instance of 'SQLite::Exception'
 # ould also need to comment in relevent lines in LoadClas12Root.C
 # add_definitions(-DRCDB_SQLITE)
 # also need the following line if not icnluded by CLASQA already
 # set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -Wl,--allow-multiple-definition")

ENDIF (DEFINED ENV{RCDB_HOME})


include_directories(${CLAS12ROOT_DIR})


ROOT_GENERATE_DICTIONARY(G__Clas12Banks ${CLASS_LIST_H} LINKDEF Clas12LinkDef.h)
add_library(Clas12Banks SHARED ${CLASS_LIST_CPP} G__Clas12Banks.cxx)

target_link_libraries(Clas12Banks ${ROOT_LIBRARIES} Hipo4)

install(TARGETS Clas12Banks
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

#FILE(GLOB ROOT_CLAS12_PCM ${CMAKE_BINARY_DIR}/Clas12Banks/*pcm)

#install (FILES  ${ROOT_CLAS12_PCM}     
#  DESTINATION "${CMAKE_INSTALL_LIBDIR}")


install (FILES  ${CMAKE_CURRENT_BINARY_DIR}/libClas12Banks_rdict.pcm    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")
