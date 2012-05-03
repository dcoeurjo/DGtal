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
 * @file KorobovSampling.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/05/01
 *
 * Header file for module KorobovSampling.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(KorobovSampling_RECURSES)
#error Recursive header files inclusion detected in KorobovSampling.h
#else // defined(KorobovSampling_RECURSES)
/** Prevents recursive inclusion of headers. */
#define KorobovSampling_RECURSES

#if !defined KorobovSampling_h
/** Prevents repeated inclusion of headers. */
#define KorobovSampling_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/domains/CDomain.h"
#include <boost/array.hpp>
#include <cmath>
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class KorobovSampling
  /**
   * Description of template class 'KorobovSampling' <p>
   * \brief Aim: Define a uniform point sampling in a given domain.
   *
   *
   */
  template <typename TDomain>
  class KorobovSampling
  {
    // ----------------------- Standard services ------------------------------
  public:

    ///Domain type
    typedef TDomain Domain;
    typedef typename Domain::Point Point;
    typedef typename Domain::Vector Vector;
    typedef typename Domain::Space::Integer Integer;


    /// domain
    BOOST_CONCEPT_ASSERT ( ( CDomain<TDomain> ) );


    KorobovSampling(const Domain &aDomain, const Integer &aSeed, const Integer &N )
    {
      mySeed = aSeed;
      myPrevious = myPrevious.diagonal(0.5);
      myN = N;

      myZ[0] = 1.0;
      myExtent = aDomain.extent();

      for(DGtal::Dimension d=1; d < Domain::dimension ; d++)
        myZ[d] = fmod(NumberTraits<Integer>::castToDouble(aSeed)*myZ[d-1] ,
                      NumberTraits<Integer>::castToDouble(myExtent[d]));
      }



    /**
     * Generate a point using a uniform random generator.
     *
     * @return a Point.
     */
    inline
    Point generate()
    {
       for(DGtal::Dimension d=0; d < Domain::dimension ; d++)
       {
         myPrevious[d] =  fmod(myPrevious[d] + myZ[d]/NumberTraits<Integer>::castToDouble(myN), 1.0);
         myPreviousDigital[d] = myPrevious[d] * myExtent[d];
       }
      return myPreviousDigital;
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
    Integer mySeed;

    ///Previous point
    typename Domain::Space::RealPoint myZ;

    ///Previous point in the [0,1]^s cube
    typename Domain::Space::RealPoint myPrevious;

    ///Previous digital point
    Point myPreviousDigital;

    ///Domain size
    Vector myExtent;

    ///Number of points
    Integer myN;


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    KorobovSampling();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    KorobovSampling ( const KorobovSampling & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    KorobovSampling & operator= ( const KorobovSampling & other );

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class KorobovSampling


  /**
   * Overloads 'operator<<' for displaying objects of class 'KorobovSampling'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'KorobovSampling' to write.
   * @return the output stream after the writing.
   */
  template <typename T>
  std::ostream&
  operator<< ( std::ostream & out, const KorobovSampling<T> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/volumes/sampling/KorobovSampling.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined KorobovSampling_h

#undef KorobovSampling_RECURSES
#endif // else defined(KorobovSampling_RECURSES)

