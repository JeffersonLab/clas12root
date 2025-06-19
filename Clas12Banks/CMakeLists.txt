# ==============================================================================
# Clas12Banks Library Definition
# ==============================================================================

set(CLAS12BANKS_TARGET_NAME Clas12Banks)

# --- Define Source Files and Headers ---
set(CLAS12BANKS_SOURCES
    helflip.cpp helonline.cpp vertdoca.cpp runconfig.cpp event.cpp ftbevent.cpp
    particle.cpp ftbparticle.cpp mcparticle.cpp mcevent.cpp mcmatch.cpp scaler.cpp
    vtp.cpp particle_detector.cpp scintillator.cpp scintextras.cpp tracker.cpp
    utracker.cpp traj.cpp forwardtagger.cpp cherenkov.cpp rich.cpp calorimeter.cpp
    calextras.cpp covmatrix.cpp region_particle.cpp region_ft.cpp region_fdet.cpp
    region_cdet.cpp region_band.cpp clas12writer.cpp clas12reader.cpp
    mesonex_trigger.cpp scaler_reader.cpp clas12databases.cpp qadb_reader.cpp
    ccdb_reader.cpp rcdb_reader.cpp
)
set(CLAS12BANKS_HEADERS
    helflip.h helonline.h vertdoca.h runconfig.h event.h ftbevent.h particle.h
    ftbparticle.h mcparticle.h mcevent.h mcmatch.h scaler.h vtp.h
    particle_detector.h scintillator.h scintextras.h tracker.h utracker.h traj.h
    forwardtagger.h cherenkov.h rich.h calorimeter.h calextras.h covmatrix.h
    region_particle.h region_ft.h region_fdet.h region_cdet.h region_band.h
    clas12writer.h clas12reader.h mesonex_trigger.h scaler_reader.h
    clas12databases.h qadb_reader.h ccdb_reader.h rcdb_reader.h
)


# --- Generate ROOT Dictionary ---
root_generate_dictionary(G__${CLAS12BANKS_TARGET_NAME}
    ${CLAS12BANKS_HEADERS}
    LINKDEF Clas12LinkDef.h
)


# --- Create the Library Target ---
add_library(${CLAS12BANKS_TARGET_NAME} SHARED
    ${CLAS12BANKS_SOURCES}
    G__${CLAS12BANKS_TARGET_NAME}.cxx
)
# This is a critical step. It defines the public API for the Clas12Banks
# library itself. It says that any target that links against Clas12Banks
# should have the current source directory added to its include paths.
# required for Clas12Root/TRcdbVals.h
target_include_directories(${CLAS12BANKS_TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)


# --- Link Dependencies to the Library ---
# The Clas12Banks library needs headers from the Clas12Root directory.
# We provide this include path directly to the target without creating a
# link dependency, which would be circular.
# Using PUBLIC ensures this path is also available to anything that uses Clas12Banks.
target_include_directories(${CLAS12BANKS_TARGET_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Clas12Root
)

# Build up a list of libraries to link
set(CLAS12BANKS_LINK_LIBRARIES PUBLIC
    ROOT::Core
    ROOT::Hist
    hipo::hipo
)
if(TARGET clas12::qadb)
    list(APPEND CLAS12BANKS_LINK_LIBRARIES clas12::qadb)
    set(CLASS_LIST_CPP ${CLASS_LIST_CPP} jsonFileMerger.cpp)
    set(CLASS_LIST_H ${CLASS_LIST_H} jsonFileMerger.h)
endif()
if(TARGET clas12::ccdb)
    list(APPEND CLAS12BANKS_LINK_LIBRARIES clas12::ccdb)
endif()
if(TARGET clas12::rcdb)
    list(APPEND CLAS12BANKS_LINK_LIBRARIES clas12::rcdb)
endif()

# Link the full list to the target
target_link_libraries(${CLAS12BANKS_TARGET_NAME} PUBLIC ${CLAS12BANKS_LINK_LIBRARIES})


# --- Installation ---
install(TARGETS ${CLAS12BANKS_TARGET_NAME}
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
