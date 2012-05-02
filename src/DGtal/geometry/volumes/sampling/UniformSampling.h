/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file UniformSampling.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/05/01
 *
 * Header file for module UniformSampling.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(UniformSampling_RECURSES)
#error Recursive header files inclusion detected in UniformSampling.h
#else // defined(UniformSampling_RECURSES)
/** Prevents recursive inclusion of headers. */
#define UniformSampling_RECURSES

#if !defined UniformSampling_h
/** Prevents repeated inclusion of headers. */
#define UniformSampling_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/domains/CDomain.h"
#include <boost/array.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class UniformSampling
  /**
   * Description of template class 'UniformSampling' <p>
   * \brief Aim: Define a uniform point sampling in a given domain.
   *
   *
   */
  template <typename TDomain, typename TPNRGEngine = boost::random::mt19937 >
  class UniformSampling
  {
    // ----------------------- Standard services ------------------------------
  public:

    ///Domain type
    typedef TDomain Domain;
    typedef TPNRGEngine PNRGEngine;
    typedef typename Domain::Point Point;
    typedef typename Domain::Space::Integer Integer;


    /// domain
    BOOST_CONCEPT_ASSERT ( ( CDomain<TDomain> ) );


    UniformSampling(const Domain &aDomain, const PNRGEngine aSeed = PNRGEngine())
    {
      mySeed = aSeed;
      for(DGtal::Dimension d=0; d < Domain::dimension ; d++)
        myDistributions[d] = boost::random::uniform_int_distribution<Integer>(aDomain.lowerBound()[d],
                                                                              aDomain.upperBound()[d] );
    }

    /** 
     * Generate a point using a uniform random generator.
     * 
     * @return a Point.
     */
    Point generate()
    {
      Point p;
      for(DGtal::Dimension d=0; d < Domain::dimension ; d++)
        p[d] = myDistributions[d](mySeed);
      
      return p;
    }

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  private:
    // ------------------------- Private Datas --------------------------------
  private:
    
    ///Random seed
    PNRGEngine mySeed;
    
    ///Random ditribution
    boost::array < boost::random::uniform_int_distribution<Integer> , 
                   Domain::dimension > myDistributions;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    UniformSampling();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    UniformSampling ( const UniformSampling & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    UniformSampling & operator= ( const UniformSampling & other );

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class UniformSampling


  /**
   * Overloads 'operator<<' for displaying objects of class 'UniformSampling'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'UniformSampling' to write.
   * @return the output stream after the writing.
   */
  template <typename T, typename E>
  std::ostream&
  operator<< ( std::ostream & out, const UniformSampling<T,E> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/volumes/sampling/UniformSampling.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined UniformSampling_h

#undef UniformSampling_RECURSES
#endif // else defined(UniformSampling_RECURSES)
