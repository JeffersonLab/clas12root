
#ROOT_GENERATE_DICTIONARY(G__Hipo4  reader.h bank.h writer.h  ntuple_writer.h ntuple_reader.h LINKDEF HipoLinkDef.h) #cant' generate dictionary for ntuple_writer with ROOT 6.22.02 see https://sft.its.cern.ch/jira/browse/ROOT-11013

ROOT_GENERATE_DICTIONARY(G__Hipo4  reader.h bank.h writer.h LINKDEF HipoLinkDef.h)

add_library(Hipo4 SHARED utils.cpp dictionary.cpp  event.cpp record.cpp recordbuilder.cpp reader.cpp bank.cpp writer.cpp ntuple_writer.cpp ntuple_reader.cpp  G__Hipo4.cxx)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(Hipo4 ${ROOT_LIBRARIES} "-undefined dynamic_lookup")
else()
  target_link_libraries(Hipo4 ${ROOT_LIBRARIES}  ${LZ4_LINK_LIBRARY})
endif()

install(TARGETS Hipo4
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

FILE(GLOB ROOT_HIPO_PCM ${CMAKE_BINARY_DIR}/hipo4/*pcm)

install (FILES ${ROOT_HIPO_PCM}   
         DESTINATION "${CMAKE_INSTALL_LIBDIR}")
