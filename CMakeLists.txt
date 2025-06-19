# ==============================================================================
# Project Preamble
# ==============================================================================
# Set a modern CMake version. This is CRITICAL. Version 3.16 or higher
# enables the modern policies needed for features like IMPORTED targets to
# work reliably. Using 3.5 forces old, buggy behavior.
cmake_minimum_required(VERSION 3.16)

# Provide a friendly error message if the user's CMake is too old.
if(${CMAKE_VERSION} VERSION_LESS 3.16)
    message(FATAL_ERROR "This project requires CMake 3.16 or higher.")
endif()
message(STATUS "Using cmake version ${CMAKE_VERSION}"  )

# Define the project name, version, and language
project(clas12root VERSION 1.9 LANGUAGES CXX)

# ==============================================================================
# Set Project-Wide Build Output Directories
# ==============================================================================
# The root_generate_dictionary command needs to know
# where to place the .pcm and .rootmap files. It checks this global variable.
# By setting it, all libraries and their corresponding ROOT dictionary files
# will be placed in a consistent "lib" directory inside your build folder.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set a modern C++ standard. C++17 is a good choice for modern ROOT.
# Currently leave ROOT to choose standard
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use PIC for shared libraries and set a default build type if none is specified.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# ==============================================================================
# Set Default Installation Prefix from Environment Variable
# ==============================================================================

# This logic sets a default CMAKE_INSTALL_PREFIX based on the CLAS12ROOT
# environment variable, but ONLY if the user has not already specified one
# on the command line (e.g., with -DCMAKE_INSTALL_PREFIX=...).
# The command line always takes priority.

if(DEFINED ENV{CLAS12ROOT})
    # This checks if the user has NOT provided a -D... flag for the prefix.
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        # We MUST use FORCE here to overwrite the default /usr/local that CMake
        # has already put in the cache. This is safe because this whole block
        # is skipped if the user provides the -D flag.
        set(CMAKE_INSTALL_PREFIX "$ENV{CLAS12ROOT}" CACHE PATH "Default install path from CLAS12ROOT env variable" FORCE)
    endif()
endif()

# This message now accurately reports the final installation prefix,
# whether it came from the default, the environment variable, or the user.
message(STATUS "Installation prefix set to: ${CMAKE_INSTALL_PREFIX}")

# ==============================================================================
# Find Dependencies
#
# We find all required packages and define IMPORTED targets for libraries that
# are not CMake-aware (like HIPO).
# ==============================================================================

# --- 1. Find ROOT ---
# This single command replaces the old system. It finds ROOT and creates the
# modern ROOT::* library targets.
# NOTE: Ensure you have sourced 'thisroot.sh' in your terminal first!
find_package(ROOT REQUIRED COMPONENTS
Core      # Fundamental ROOT classes (TObject, TString, TTree, basic I/O)
    RIO       # ROOT I/O system (reading/writing .root files, crucial for TTree)
    Hist      # Histogramming (TH1, TH2, TGraph, etc.)
    Graf      # Basic graphics primitives (TPolyLine, TMarker, etc.)
    Graf3d    # 3D Graphics primitives (if you use any 3D drawing)
    Gpad      # Graphics Pads (TPad), crucial for canvases
    Gui       # GUI elements (buttons, menus, dialogs - if you have a custom GUI)
    MathCore  # Basic math functions (e.g., TMath, TLorentzVector)
    # More advanced/specific components you might need based on your code:
    Tree      # TTree and related classes (often pulled in by Core, but good to be explicit)
    Geom      # If you use ROOT's geometry package (TGeo)
    GenVector # ROOT's vector and physics vector classes (TLorentzVector, etc.)
    Rint      # The ROOT interactive interpreter. Usually not needed for compiled executables.
                # Only include this if you explicitly link to the interactive shell for some reason.
                # It can sometimes pull in many dependencies.
)
include(${ROOT_USE_FILE})

# --- 2. Find HIPO4 ---
# Instead of manually setting variables, we check for the HIPO environment
# variable and create a proper, modern CMake target from it.
message(STATUS "Checking for HIPO4 library...")
if(DEFINED ENV{HIPO})
  set(HIPO_INSTALL_DIR $ENV{HIPO})
  message(STATUS "Found HIPO4 at: ${HIPO_INSTALL_DIR}")

  # Create a modern INTERFACE library target for HIPO.
  # This encapsulates its usage requirements in a single target: "hipo::hipo"
  add_library(hipo::hipo INTERFACE IMPORTED)
  target_include_directories(hipo::hipo INTERFACE ${HIPO_INSTALL_DIR}/include)
  target_link_libraries(hipo::hipo INTERFACE ${HIPO_INSTALL_DIR}/lib/libhipo4.so)

else()
  message(FATAL_ERROR "HIPO environment variable not set. Please set HIPO to point to your hipo4 installation directory.")
endif()

# --- Find External Dependencies (QADB, CCDB, RCDB) ---
# This logic is now centralized here so that all subdirectories can see
# the dependency targets we create.

# --- 3. Find QADB ---
if(DEFINED ENV{QADB})
    message(STATUS "Configuring dependency: QADB")
    add_library(clas12::qadb INTERFACE IMPORTED)
    target_compile_definitions(clas12::qadb INTERFACE -DCLAS_QADB)
    target_include_directories(clas12::qadb INTERFACE $ENV{QADB}/srcC/include)
    target_include_directories(clas12::qadb INTERFACE $ENV{QADB}/srcC/rapidjson/include)
    include_directories($ENV{QADB}/srcC/include)
    include_directories($ENV{QADB}/srcC/rapidjson/include)
endif()


# --- 4. Find CCDB ---
if(DEFINED ENV{CCDB_HOME})
    message(STATUS "Configuring dependency: CCDB")
    add_library(clas12::ccdb INTERFACE IMPORTED)
    target_include_directories(clas12::ccdb INTERFACE $ENV{CCDB_HOME}/include)
    target_link_libraries(clas12::ccdb INTERFACE $ENV{CCDB_HOME}/lib/libccdb.so) # Assumes .so, adjust if needed
    target_compile_definitions(clas12::ccdb INTERFACE -DCLAS_CCDB)
    include_directories($ENV{CCDB_HOME}/include)
 endif()


# --- 5. Find RCDB and MySQL ---
if(DEFINED ENV{RCDB_HOME})
    message(STATUS "Configuring dependency: RCDB")
    add_library(clas12::rcdb INTERFACE IMPORTED)
    target_include_directories(clas12::rcdb INTERFACE $ENV{RCDB_HOME}/cpp/include)
    target_compile_definitions(clas12::rcdb INTERFACE -DCLAS_RCDB)
    include_directories($ENV{RCDB_HOME}/cpp/include)

    find_package(MySQL QUIET)


    if(MySQL_FOUND)
        message(STATUS "Found MySQL for RCDB support")
    else()
     message(STATUS "MySQL not found via find_package. Attempting to locate headers and library manually.")

          # --- Manual Search for MySQL ---
     	  # Find MySQL Header (mysql.h)
     	  FIND_PATH(MySQL_INCLUDE_DIR
		mysql.h
        	PATHS
        	/usr/local/include/mysql
        	/usr/include/mysql
        	/opt/homebrew/opt/mysql-client/include # Common Homebrew path
        	/usr/local/Cellar/mysql-client/*/include # More generic Homebrew path
        	"$ENV{MYSQL_HOME}/include" # Check an environment variable hint
        	"$ENV{MYSQL_ROOT}/include"
        	NO_DEFAULT_PATH # Only search the paths explicitly listed
    	  )

    	  # Find MySQL Library (libmysqlclient)
    	  FIND_LIBRARY(MySQL_LIBRARIES
		NAMES mysqlclient libmysqlclient # Common names for the library
        	PATHS
        	/usr/local/lib
        	/usr/lib
        	/usr/lib/x86_64-linux-gnu # Common Linux specific path
        	/opt/homebrew/opt/mysql-client/lib # Common Homebrew path
        	/usr/local/Cellar/mysql-client/*/lib # More generic Homebrew path
        	"$ENV{MYSQL_HOME}/lib"
        	"$ENV{MYSQL_ROOT}/lib"
        	NO_DEFAULT_PATH # Only search the paths explicitly listed
    	  )

    	  # Check if both header and library were found manually
    	  if (MySQL_INCLUDE_DIR AND MySQL_LIBRARIES)
             message(STATUS "MySQL manually found. Header: ${MySQL_INCLUDE_DIR}, Library: ${MySQL_LIBRARIES}")
    	  else()
          # If either part of the manual search failed, report an error
               message(FATAL_ERROR "MySQL development files not found.
               Please ensure 'libmysqlclient-dev' (Debian/Ubuntu) or 'mysql-devel' (Fedora/RHEL) or 'mysql-client' (Homebrew) is installed.
               You may also need to set CMAKE_PREFIX_PATH or MYSQL_ROOT_DIR if installed in a non-standard location."
               )
    	  endif()

    endif()
    if (MySQL_INCLUDE_DIR AND MySQL_LIBRARIES)
        target_include_directories(clas12::rcdb INTERFACE ${MySQL_INCLUDE_DIR})
        target_link_libraries(clas12::rcdb INTERFACE ${MySQL_LIBRARIES})
        target_compile_definitions(clas12::rcdb INTERFACE -DRCDB_MYSQL)
    	include_directories(${MySQL_INCLUDE_DIR})
    endif()
endif()


# ==============================================================================
# Installation Rules
#
# This section defines how to install the project's products (libraries,
# executables, headers).
# ==============================================================================
include(GNUInstallDirs)

# ==============================================================================
# Configure Subdirectories
#
# We add our sub-projects. The global include_directories() calls have been
# removed. Each subdirectory is now responsible for defining its own targets
# and public include paths.
# ==============================================================================

set(HIPOROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/hipo4)

#include_directories(${HIPOROOT_DIR})
#add_subdirectory (${HIPOROOT_DIR})
add_subdirectory(hipo4)
add_subdirectory(Clas12Banks)
add_subdirectory(Clas12Root)



# NOTE: You need to replace "your_lib_target" and "your_exe_target" with the
# actual names of the targets defined in your subdirectories' CMakeLists.txt files.
# You can have multiple install() commands.

# Example of installing a library:
# install(TARGETS Hipo4
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# Example of installing an executable:
# install(TARGETS your_exe_target
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# Example of installing a public header file or directory:
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Clas12Root/include/
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
