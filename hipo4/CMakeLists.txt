
# ==============================================================================
# Hipo4 Wrapper Library Definition
#
# This file defines a small wrapper library that provides ROOT dictionary
# support for HIPO ntuple classes.
# ==============================================================================

# For clarity, we define the name of the library target we are building.
set(HIPO4_WRAPPER_TARGET_NAME Hipo4)

# --- 1. Define Source Files and Headers for This Library ---
set(HIPO4_WRAPPER_SOURCES
    ntuple_writer.cpp
    ntuple_reader.cpp
)
set(HIPO4_WRAPPER_HEADERS
    ntuple_writer.h
    ntuple_reader.h
)

# --- 2. Generate ROOT Dictionary ---
# Replaced the deprecated ROOT_GENERATE_DICTIONARY with the modern function.
##include_directories(${HIPO_INCLUDE_DIR})
root_generate_dictionary(G__${HIPO4_WRAPPER_TARGET_NAME}
    ${HIPO4_WRAPPER_HEADERS}
    LINKDEF HipoLinkDef.h
)

# --- 3. Create the Library Target ---
add_library(${HIPO4_WRAPPER_TARGET_NAME} SHARED  ${HIPO4_WRAPPER_SOURCES} G__${HIPO4_WRAPPER_TARGET_NAME}.cxx )

# --- 4. Define Public Include Directory ---
# This is a critical step. It declares that any target that links against our
# new "Hipo4" library should automatically have the current source directory
# added to its include paths. This replaces the old global include_directories().
target_include_directories(${HIPO4_WRAPPER_TARGET_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)


# --- 5. Link Dependencies to the Library ---
# We link our library against its dependencies using modern targets.
target_link_libraries(${HIPO4_WRAPPER_TARGET_NAME} PUBLIC
    # Link the HIPO target from the parent scope
    hipo::hipo

    # Link the required ROOT components
    ROOT::Core
)

# Handle platform-specific linker flags for macOS
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # On macOS, "-undefined dynamic_lookup" can resolve certain symbol issues
    # at runtime. We add this as a PUBLIC link option so any executable that
    # uses this library will also receive the flag.
    target_link_options(${HIPO4_WRAPPER_TARGET_NAME} PUBLIC "-undefined dynamic_lookup")
endif()


# --- 6. Installation ---
install(TARGETS ${HIPO4_WRAPPER_TARGET_NAME}
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    #MODULE DESTINATION "${CMAKE_INSTALL_LIBDIR}" # <-- This handles the .pcm file, should work but does not
)

install(TARGETS ${HIPO4_WRAPPER_TARGET_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")


# This command was likely missing its DESTINATION argument.
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${HIPO4_WRAPPER_TARGET_NAME}_rdict.pcm
#    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${HIPO4_WRAPPER_TARGET_NAME}_rdict.pcm
#    DESTINATION "lib"
#)
