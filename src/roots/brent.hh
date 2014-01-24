//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   roots/brent.hh
 * \author Kent Budge
 * \date   Tue Aug 17 15:30:23 2004
 * \brief  Find minimum of a function.
 * \note   Copyright (C) 2006-2014 Los Alamos National Security, LLC
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#ifndef roots_brent_hh
#define roots_brent_hh

#include "brent.hh"
#include "ds++/Assert.hh"
#include "ds++/sign.hh"

#include <cmath>
#include <limits>
#include <vector>

namespace rtt_roots
{

//---------------------------------------------------------------------------//
/*!
 * Find minimum of a function.
 *
 * \arg \a Function A function type supporting <code>double
 * operator()(double)</code>.
 *
 * \param[in] ax Lower bound of bracket interval.
 * \param[in] bx Upper bound of bracket interval.
 * \param[in] cx Estimate of minimum location within bracket.
 * \param[in] f Function to be minimized
 * \param[in] tol Fractional tolerance on location of minimum.
 * \param[out] xmin Location of minimum
 *
 * \return Value of minimum.
 */

template <class Function>
double brent(double const ax,
             double const bx,
             double const cx,
             Function f,
             double const tol,
             double &xmin)
{
    using std::numeric_limits;

    unsigned const ITMAX = 100;
    double const ZEPS = numeric_limits<double>::epsilon()*1.0e-3;
    double const CGOLD = 0.3819660;

    double d = 0.0;
    double e = 0.0;
    
    double a = (ax<cx? ax : cx);
    double b = (ax>cx? ax : cx);
    double x = bx, w = bx, v = bx;
    double fx = f(x);
    double fw = fx, fv = fx;
    double u, fu;
    for (unsigned iter=0; iter<ITMAX; ++iter)
    {
        double const xm = 0.5*(a+b);
        double const tol1 = tol*fabs(x)+ZEPS;
        double const tol2 = 2*tol1;
        if (fabs(x-xm) <= (tol2-0.5*(b-a)))
        {
            xmin = x;
            return fx;
        }
        if (fabs(e)>tol1)
        {
            double const r = (x-w)*(fx-fv);
            double q = (x-v)*(fx-fw);
            double p = (x-v)*q-(x-w)*r;
            q = 2*(q-r);
            if (q>0)
            {
                p = -p;
            }
            q = fabs(q);
            double const etemp = e;
            e = d;
            if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
            {
                e = (x>=xm? a-x : b-x);
                d = CGOLD*e;
            }
            else
            {
                d = p/q;
                u = x+d;
                if (u-a < tol2 || b-u < tol2)
                {
                    d = rtt_dsxx::sign(tol1, xm-x);
                }
            }
        }
        else
        {
            e = (x>=xm? a-x : b-x);
            d = CGOLD*e;
        }
        u = (fabs(d) >= tol1 ? x+d : x+rtt_dsxx::sign(tol1,d));
        fu = f(u);
        if (fu <= fx)
        {
            if (u >= x)
            {
                a=x;
            }
            else
            {
                b=x;
            }
            v = w; w = x; x = u;
            fv = fw; fw = fx; fx = fu;
        }
        else
        {
            if (u<x)
            {
                a = u;
            }
            else
            {
                b = u;
            }
            if (fu <= fw || w == x)
            {
                v = w;
                w = u;
                fv = fw;
                fw = fu;
            }
            else if (fu<=fv || v==x || v==w)
            {
                v = u;
                fv = fu;
            }
        }
    }
    throw std::range_error("Too many iterations in brent");
    // xmin = x;
    return fx;
}

} // end namespace rtt_roots

#endif // roots_brent_hh

//---------------------------------------------------------------------------//
//              end of roots/brent.hh
//---------------------------------------------------------------------------//
