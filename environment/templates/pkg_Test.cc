//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   <tpkg>/<spkg>_test.cc
 * \author <user>
 * \date   <date>
 * \brief  <start>
 * \note   Copyright (C) 2006-2014 Los Alamos National Security, LLC
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include <iostream>
#include "<spkg>_test.hh"

namespace <namespace>_test
{

//===========================================================================//
// PASS/FAILURE
//===========================================================================//

bool fail(int line)
{
    std::cout << "Test: failed on line " << line << std::endl;
    passed = false;
    return false;
}

//---------------------------------------------------------------------------//

bool fail(int line, char *file)
{
    std::cout << "Test: failed on line " << line << " in " << file
              << std::endl;
    passed = false;
    return false;
}

//---------------------------------------------------------------------------//

bool pass_msg(const std::string &passmsg)
{
    std::cout << "Test: passed" << std::endl;
    std::cout << "     " << passmsg << std::endl;
    return true;
}

//---------------------------------------------------------------------------//

bool fail_msg(const std::string &failmsg)
{
    std::cout << "Test: failed" << std::endl;
    std::cout << "     " << failmsg << std::endl;
    passed = false;
    return false;
}

//---------------------------------------------------------------------------//

void unit_test(const bool pass, int line, char *file)
{
    if ( ! pass )
    {
        fail(line, file);
    }
}

//---------------------------------------------------------------------------//
// BOOLEAN PASS FLAG
//---------------------------------------------------------------------------//

bool passed = true;

} // end namespace <namespace>_test

//---------------------------------------------------------------------------//
//               end of <spkg>_test.cc
//---------------------------------------------------------------------------//
