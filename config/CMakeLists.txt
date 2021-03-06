#-----------------------------*-cmake-*----------------------------------------#
# file   config/CMakeLists.txt
# author Kelly Thompson <kgt@lanl.gov>
# date   2010 Nov 2
# brief  Install the config directory in the target directory
# note   Copyright (C) 2016-2020, Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.17.0)
project( config )

include(CMakePackageConfigHelpers)

file( GLOB CMake_src *.cmake )
file( GLOB config_helper_sh *.sh )
file( GLOB Python_src *.py )
set( CMake_in
   cmake_uninstall.cmake.in
   doxygen_config.in )
set( CMake_AFSD CMakeAddFortranSubdirectory/config_cafs_proj.cmake.in  )

# include(debug_macros)
# echo_all_cmake_variable_values()

# ---------------------------------------------------------------------------- #
# Prepare data that will be saved in draco-config.cmake
# ---------------------------------------------------------------------------- #

string( TOUPPER "${CMAKE_BUILD_TYPE}" upper_build_type )
set( Draco_CXX_COMPILER_FLAGS
   "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${upper_build_type}}" )
set( Draco_C_COMPILER_FLAGS
   "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${upper_build_type}}" )
set( Draco_Fortran_COMPILER_FLAGS
   "${CMAKE_Fortran_FLAGS} ${CMAKE_Fortran_FLAGS_${upper_build_type}}" )

# Publish the active build_types (A subset of CMAKE_CONFIGURATION_TYPES)
if( CMAKE_CONFIGURATION_TYPES )
   unset( DRACO_CONFIGURATION_TYPES CACHE)
   foreach( cfg ${CMAKE_CONFIGURATION_TYPES} )
      if( EXISTS ${Draco_BINARY_DIR}/${cfg} )
         if( "${DRACO_CONFIGURATION_TYPES}x" STREQUAL "x" )
            set( DRACO_CONFIGURATION_TYPES "${cfg}" )
         else()
            list( APPEND DRACO_CONFIGURATION_TYPES ${cfg} )
         endif()
      endif()
   endforeach()
   set( DRACO_CONFIGURATION_TYPES "${DRACO_CONFIGURATION_TYPES}"
      CACHE STRING "Available multiconfig builds." )
endif()

# What MPI import targets should be found by clients (captured in
# draco-config.cmake).
if ( "${DRACO_C4}" STREQUAL "MPI" )
  # c4 requires MPI::MPI_CXX if DRACO_C4==MPI.
  set( Draco_MPI_LANG_LIST "CXX" )
  # If Draco found other MPI imported targets, then ensure they are defined for
  # client software packages (e.g.: MPI::MPI_Fortran)
  foreach( lang C Fortran )
    if( TARGET MPI::MPI_${lang} )
      list( APPEND Draco_MPI_LANG_LIST ${lang} )
      # set( Draco_MPI_LANG_LIST "${Draco_MPI_LANG_LIST} ${lang}" )
    endif()
  endforeach()
endif()

set(Draco_TPL_LIST GSL Random123) # except MPI (which is treated separately).
if( TARGET COMPTON::compton )
  list(APPEND Draco_TPL_LIST COMPTON )
endif()
if( TARGET EOSPAC::eospac )
  list(APPEND Draco_TPL_LIST EOSPAC )
endif()

# Process the config file
configure_file( draco-config-install.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/draco-config.cmake" @ONLY)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/draco-config-version.cmake"
  VERSION ${Draco_VERSION}
  COMPATIBILITY AnyNewerVersion )

# Install scripts and macros to make them available by other projects.
set( file_list
  ${CMake_src}
  ${CMake_in}
  ${Python_src}
  ${CMAKE_CURRENT_BINARY_DIR}/draco-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/draco-config-version.cmake
  ${Draco_SOURCE_DIR}/autodoc/html/doxygen.css )

install( FILES ${file_list}  DESTINATION cmake )
install( FILES ${CMake_AFSD} DESTINATION cmake/CMakeAddFortranSubdirectory )
install( PROGRAMS ${config_helper_sh} DESTINATION cmake )

##---------------------------------------------------------------------------##
## End of config/CMakeLists.txt
##---------------------------------------------------------------------------##
