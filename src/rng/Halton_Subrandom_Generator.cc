//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   rng/Halton_Subrandom_Generator.cc
 * \author Kent Budge
 * \brief  Define methods of class Halton_Subrandom_Generator
 * \note   � Copyright 2006-2014 LANSLLC All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include "ds++/Assert.hh"
#include "Halton_Subrandom_Generator.hh"

namespace rtt_rng
{

//---------------------------------------------------------------------------//
/*!
 * \param count Dimension of the subrandom vector generated by this object.
 */
Halton_Subrandom_Generator::Halton_Subrandom_Generator(unsigned const count)
    :
    Subrandom_Generator(count),
    sequences_()
{
    /* empty */
}

//---------------------------------------------------------------------------//
void Halton_Subrandom_Generator::shift_vector()
{
    ++count_;
    unsigned const N = sequences_.size();
    for (unsigned i=0; i<N; ++i)
    {
        sequences_[i].shift();
    }
    element_ = 0;
}

//---------------------------------------------------------------------------//
double Halton_Subrandom_Generator::shift()
{
    if (element_ == sequences_.size())
    {
        sequences_.push_back(Halton_Sequence(sequences_.size(), count_));
    }
    double const Result = sequences_[element_].lookahead();
    ++element_;
    return Result;
}

} // end namespace rtt_rng

//---------------------------------------------------------------------------//
//                 end of Halton_Subrandom_Generator.cc
//---------------------------------------------------------------------------//
