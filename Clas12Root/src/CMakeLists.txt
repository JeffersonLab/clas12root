# ==============================================================================
# Executable Definitions
#
# This file defines the final executables for the project.
# ==============================================================================

# List all the executables we want to build from the source files in this directory.
set(CLAS12_EXECUTABLES
    clas12root
    clas12proof
    particleDraw
    particleTree
    makeHipoSelector
)

# Loop over the list to create and link each executable.
foreach(EXEC_NAME ${CLAS12_EXECUTABLES})

    # 1. Add the executable target from its corresponding .cpp file.
    add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)

    # 2. Link the executable to the libraries it needs.
    # By linking to the Clas12Root target (defined in the parent directory),
    # this executable will automatically inherit all the necessary link flags
    # and include paths for ROOT, HIPO, RCDB, etc.
    target_link_libraries(${EXEC_NAME} PRIVATE
        Clas12Root  # <-- This is the most important link!
    )

    # 3. Install the executable target.
    install(TARGETS ${EXEC_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )

endforeach()
