//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   diagnostics/draco_info.cc
 * \author Kelly Thompson
 * \date   Wednesday, Nov 07, 2012, 18:49 pm
 * \brief  Small executable that prints the version and copyright strings.
 * \note   Copyright (C) 2012-2014 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id: tstScalarUnitTest.cc 6864 2012-11-08 01:34:45Z kellyt $
//---------------------------------------------------------------------------//

#include "diagnostics/config.h"
#include "c4/config.h"
#include "ds++/Release.hh"
#include "ds++/UnitTest.hh"
#include "ds++/Assert.hh"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm> // tolower

//---------------------------------------------------------------------------//
// Draco-6_5_20121113, build date 2012/11/13; build type: DEBUG; DBC: 7
//
// Draco Contributers: 
//     Kelly G. Thompson, Kent G. Budge, Tom M. Evans,
//     Rob Lowrie, B. Todd Adams, Mike W. Buksas,
//     James S. Warsa, John McGhee, Gabriel M. Rockefeller,
//     Paul J. Henning, Randy M. Roberts, Seth R. Johnson,
//     Allan B. Wollaber, Peter Ahrens, Jeff Furnish,
//     Paul W. Talbot, Jae H. Chang, and Benjamin K. Bergen.
//
// Copyright (C) 1995-2014 LANS, LLC
// Build information:
//     Library type   : shared
//     System type    : Linux
//     CUDA support   : disabled
//     MPI support    : enabled
//       mpirun cmd   : mpirun --mca mpi_paffinity_alone 0 -np 
//     OpenMPI support: enabled
//     Diagnostics    : disabled
//     Diagnostics Timing: disabled
//     C++11 Support  : enabled
//       Feature list : HAS_CXX11_AUTO 
//                      HAS_CXX11_NULLPTR 
//                      HAS_CXX11_LAMBDA 
//                      HAS_CXX11_STATIC_ASSERT 
//                      HAS_CXX11_SHARED_PTR 

int main( int /*argc*/, char *argv[] )
{
    using std::cout;
    using std::endl;
    try
    {
        // Print version and copyright information to the screen:
        cout << "\n"
             << rtt_dsxx::release() << "\n\n"
             << rtt_dsxx::copyright() << endl;

//---------------------------------------------------------------------------//
// Build Information
//---------------------------------------------------------------------------//

        // Create a string to hold build_type and normalize the case.
        std::string build_type( CMAKE_BUILD_TYPE );
        std::transform( build_type.begin(), build_type.end(),
                        build_type.begin(), ::tolower);
        build_type[0] = ::toupper(build_type[0]);
        
        cout << "Build information:"
             << "\n    Build type     : " << build_type
             << "\n    Library type   : "
#ifdef DRACO_SHARED_LIBS
             << "shared"
#else
             << "static"
#endif
             << "\n    System type    : "
#if DRACO_UNAME == Linux
             << "Linux"
#else
             << "Unknown"
#endif
             << "\n    CUDA support   : "
#ifdef HAVE_CUDA
             << "enabled"
#else
             << "disabled"
#endif
           
             << "\n    MPI support    : "
#ifdef C4_MPI
             << "enabled"
             << "\n      mpirun cmd   : " << C4_MPICMD
#else
             << "disabled (c4 scalar mode)"
#endif
             << "\n    OpenMPI support: "
#if USE_OPENMP == ON
             << "enabled"
#else
             << "disabled (c4 scalar mode)"
#endif
             << "\n    Diagnostics    : "
#ifdef DRACO_DIAGNOSTICS
             << DRACO_DIAGNOSTICS
#else
             << "disabled"
#endif
             << "\n    Diagnostics Timing: " 
#ifdef DRACO_TIMING
             << "enabled"
#else
             << "disabled"
#endif
            
//---------------------------------------------------------------------------//
// C++11 Features
//---------------------------------------------------------------------------//
            
             << "\n    C++11 Support  : "
#ifdef DRACO_ENABLE_CXX11
             << "enabled"
             << "\n      Feature list : ";

        std::vector<std::string> cxx11_features(
            rtt_dsxx::UnitTest::tokenize( CXX11_FEATURE_LIST, ";", false ) );
        for( size_t i=0; i<cxx11_features.size(); ++i )
            if( i==0 )
                cout << cxx11_features[i];
            else
                cout << "\n                     " << cxx11_features[i];
#else
            << "disabled";
#endif

            // Compilers and Flags
            
        cout << "\n    CXX Compiler      : " << CMAKE_CXX_COMPILER
             << "\n    CXX_FLAGS         : " << CMAKE_CXX_FLAGS;
        if( build_type == std::string("Release") )
            cout << " " << CMAKE_CXX_FLAGS_RELEASE;
        else if( build_type == std::string("Release") )
            cout << " " << CMAKE_CXX_FLAGS_DEBUG;

        cout << "\n    C Compiler        : " << CMAKE_C_COMPILER
             << "\n    C_FLAGS           : " << CMAKE_C_FLAGS;
        if( build_type == std::string("Release") )
            cout << " " << CMAKE_C_FLAGS_RELEASE;
        else if( build_type == std::string("Release") )
            cout << " " << CMAKE_C_FLAGS_DEBUG;
#ifdef CMAKE_Fortran_COMPILER
        cout << "\n    Fortran Compiler  : " << CMAKE_Fortran_COMPILER
             << "\n    Fortran_FLAGS     : " << CMAKE_Fortran_FLAGS;
        if( build_type == std::string("Release") )
            cout << " " << CMAKE_Fortran_FLAGS_RELEASE;
        else if( build_type == std::string("Release") )
            cout << " " << CMAKE_Fortran_FLAGS_DEBUG;
#endif        
        cout << "\n" << endl;
    }
    catch( rtt_dsxx::assertion &err )
    {
        std::string msg = err.what();
        std::cout << "ERROR: While running " << argv[0] << ", "
             << err.what() << std::endl;;
        return 1;
    }
    catch( std::exception &err )
    {
        std::cout << "ERROR: While running " << argv[0] << ", "
             << err.what() << std::endl;;
        return 1;
    }
    catch( ... )
    {
        std::cout << "ERROR: While running " << argv[0] << ", " 
             << "An unknown C++ exception was thrown" << std::endl;;
        return 1;
    }

    return 0;
}   

//---------------------------------------------------------------------------//
// end of draco_info.cc
//---------------------------------------------------------------------------//
