ROOT_GENERATE_DICTIONARY(G__Clas12Banks   helflip.h  helonline.h runconfig.h event.h ftbevent.h particle.h ftbparticle.h mcparticle.h scaler.h vtp.h particle_detector.h scintillator.h tracker.h traj.h forwardtagger.h cherenkov.h calorimeter.h covmatrix.h region_particle.h region_ft.h region_fdet.h region_cdet.h clas12reader.h  LINKDEF Clas12LinkDef.h)

add_library(Clas12Banks SHARED  helflip.cpp helonline.cpp runconfig.cpp event.cpp ftbevent.cpp particle.cpp ftbparticle.cpp mcparticle.cpp scaler.cpp vtp.cpp particle_detector.cpp scintillator.cpp tracker.cpp traj.cpp forwardtagger.cpp cherenkov.cpp calorimeter.cpp covmatrix.cpp region_particle.cpp region_ft.cpp region_fdet.cpp region_cdet.cpp clas12reader.cpp   G__Clas12Banks.cxx)

target_link_libraries(Clas12Banks ${ROOT_LIBRARIES} Hipo4)


install(TARGETS Clas12Banks
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_CLAS12_PCM ${CMAKE_BINARY_DIR}/Clas12Banks/*pcm)

install (FILES  ${ROOT_CLAS12_PCM}     
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")


