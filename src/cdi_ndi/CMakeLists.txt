#-----------------------------*-cmake-*----------------------------------------#
# file   cdi_ndi/CMakeLists.txt
# brief  Instructions for building cdi_ndi Makefiles.
# note   Copyright (C) 2020 Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.17.0)
project( cdi_ndi CXX )

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #

file( GLOB sources *.cc )
file( GLOB headers *.hh )
list( APPEND headers ${PROJECT_BINARY_DIR}/cdi_ndi/config.h )
set(deps Lib_cdi Lib_rng Lib_units)

# If NDI is found, set the associated variables. Otherwise, the cdi_ndi lib will
# be built with stubbed-out routines.
if(TARGET NDI::ndi)
  list(APPEND deps NDI::ndi)

  if( NOT DEFINED NDI_ROOT_DIR AND DEFINED $ENV{NDI_ROOT_DIR})
    set(NDI_ROOT_DIR $ENV{NDI_ROOT_DIR})
  endif()

  if( NOT DEFINED NDI_ROOT_DIR )
    # if not set in the environment (by modulefile?), set NDI_ROOT_DIR to the
    # prefix_path for NDI
    get_filename_component(NDI_ROOT_DIR "${NDI_LIBRARY}" DIRECTORY)
    get_filename_component(NDI_ROOT_DIR "${NDI_LIBRARY}" DIRECTORY CACHE)
  endif()

  # Location of gendir and gendir.all files.
  if( NOT DEFINED NDI_DATA_DIR OR NOT EXISTS "${NDI_DATA_DIR}" )
    if( DEFINED ENV{NDI_GENDIR_PATH} AND EXISTS "$ENV{NDI_GENDIR_PATH}" )
      get_filename_component( NDI_DATA_DIR "$ENV{NDI_GENDIR_PATH}" DIRECTORY )
    elseif( EXISTS "${NDI_ROOT_DIR}/share/gendir" )
      set( NDI_DATA_DIR "${NDI_ROOT_DIR}/share" )
    else()
      message( FATAL_ERROR "Could not find NDI data file gendir.")
    endif()
  endif()
  set( NDI_DATA_DIR "${NDI_DATA_DIR}" CACHE PATH
    "Location of NDI gendir file." )
else()
  set(NDI_DATA_DIR "notfound")
endif()

# ---------------------------------------------------------------------------- #
# Generate config.h (only occurs when cmake is run)
# ---------------------------------------------------------------------------- #
configure_file( config.h.in ${PROJECT_BINARY_DIR}/cdi_ndi/config.h )

# ---------------------------------------------------------------------------- #
# Build package library
# ---------------------------------------------------------------------------- #
add_component_library(
   TARGET       Lib_cdi_ndi
   TARGET_DEPS  "${deps}"
   LIBRARY_NAME cdi_ndi
   SOURCES      "${sources}"
   HEADERS      "${headers}" )

target_include_directories( Lib_cdi_ndi
   PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}> )

if(TARGET NDI::ndi)
  target_include_directories( Lib_cdi_ndi PUBLIC ${NDI_INCLUDE_DIR} )
endif()

# ---------------------------------------------------------------------------- #
# Installation instructions
# ---------------------------------------------------------------------------- #

install( TARGETS Lib_cdi_ndi  EXPORT draco-targets DESTINATION
  ${DBSCFGDIR}lib )
install( FILES ${headers} DESTINATION ${DBSCFGDIR}include/cdi_ndi )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  install(FILES $<TARGET_PDB_FILE:Lib_cdi_ndi> DESTINATION ${DBSCFGDIR}lib
    OPTIONAL)
endif()

# ---------------------------------------------------------------------------- #
# Unit tests
# ---------------------------------------------------------------------------- #

if( BUILD_TESTING AND NOT DEFINED ENV{APPVEYOR} )
  add_subdirectory( test )
endif()

# ---------------------------------------------------------------------------- #
# Autodoc
# ---------------------------------------------------------------------------- #
process_autodoc_pages()

# ---------------------------------------------------------------------------- #
# End cdi_ndi/CMakeLists.txt
# ---------------------------------------------------------------------------- #
