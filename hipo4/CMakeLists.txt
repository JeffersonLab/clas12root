
include_directories(${HIPO_INCLUDE_DIR})

ROOT_GENERATE_DICTIONARY(G__Hipo4  ntuple_writer.h ntuple_reader.h LINKDEF HipoLinkDef.h) 
add_library(Hipo4 SHARED ntuple_writer.cpp ntuple_reader.cpp  G__Hipo4.cxx)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(Hipo4 ${ROOT_LIBRARIES}  ROOT::Core "-undefined dynamic_lookup")
else()
  target_link_libraries(Hipo4 ${ROOT_LIBRARIES}  ROOT::Core ${HIPO_LIBS})
endif()

install(TARGETS Hipo4
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")


install (FILES  ${CMAKE_CURRENT_BINARY_DIR}/libHipo4_rdict.pcm    
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

