set(CLAS12ROOT Clas12Root)

add_executable (clas12root clas12root.cpp)
target_link_libraries(clas12root  ${ROOT_LIBRARIES})

add_executable (clas12proof clas12proof.cpp)
target_link_libraries(clas12proof ${ROOT_LIBRARIES})

add_executable (particleDraw particleDraw.cpp)
target_link_libraries(particleDraw ${ROOT_LIBRARIES})

add_executable (particleTree particleTree.cpp)
target_link_libraries(particleTree ${ROOT_LIBRARIES})
install (TARGETS clas12root clas12proof particleDraw particleTree DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable (makeHipoSelector makeHipoSelector.cpp)
target_link_libraries(makeHipoSelector ${ROOT_LIBRARIES})


install (TARGETS makeHipoSelector DESTINATION ${CMAKE_INSTALL_BINDIR})
