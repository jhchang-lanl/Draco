//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   rtt_c4/Functors.hh
 * \author Thomas M. Evans
 * \date   Fri Sep 30 12:54:09 2005
 * \brief  Functors used in rtt_c4.
 * \note   Copyright (C) 2004-2014 Los Alamos National Security, LLC.
 *         All rights reserved.
 *
 * Functors for use with accumulatev.
 */
//---------------------------------------------------------------------------//
// $Id: Functors.hh 6056 2012-06-19 19:05:27Z kellyt $
//---------------------------------------------------------------------------//

#ifndef rtt_c4_Functors_hh
#define rtt_c4_Functors_hh

#include <functional>

namespace rtt_c4
{

//---------------------------------------------------------------------------//
//! A binary functor to return the maximum of two items
/** 
 * 
 * @param x, the first operand
 * @param y, the second operand
 * @return the greater value of x or y
 */
template <class T>
struct max : public std::binary_function<T,T,T>
{
    T operator()(const T& x, const T& y) const
    { 
        return (x > y)  ? x : y;
    }
};

//---------------------------------------------------------------------------//
//! A binary functor to find the minimum of two items
/** 
 * 
 * @param x, the first operand
 * @param y, the second operand
 * @return the lesser value of x or y
 */  
template <class T>
struct min : public std::binary_function<T,T,T>
{
    T operator()(const T& x, const T& y) const
    { 
        return (x < y) ? x : y;
    }
};

} // end namespace rtt_c4

#endif // rtt_c4_Functors_hh

//---------------------------------------------------------------------------//
// end of rtt_c4/Functors.hh
//---------------------------------------------------------------------------//
