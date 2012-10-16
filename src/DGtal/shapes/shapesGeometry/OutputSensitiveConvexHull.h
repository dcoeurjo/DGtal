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
 * @file OutputSensitiveConvexHull.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/05/04
 *
 * Header file for module OutputSensitiveConvexHull.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(OutputSensitiveConvexHull_RECURSES)
#error Recursive header files inclusion detected in OutputSensitiveConvexHull.h
#else // defined(OutputSensitiveConvexHull_RECURSES)
/** Prevents recursive inclusion of headers. */
#define OutputSensitiveConvexHull_RECURSES

#if !defined OutputSensitiveConvexHull_h
/** Prevents repeated inclusion of headers. */
#define OutputSensitiveConvexHull_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/math/arithmetic/CPositiveIrreducibleFraction.h"
#include "DGtal/shapes/CDigitalOrientedShape.h"

//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class OutputSensitiveConvexHull
  /**
   * Description of template class 'OutputSensitiveConvexHull' <p>
   * \brief Aim:
   *
   *
   * @tparam TShape any model of digital oriented shape
   * @tparam TSampler any model of point sampler
   */
  template <typename TShape, typename TSampler>
  class OutputSensitiveConvexHull
  {
    // ----------------------- Standard services ------------------------------
  public:

    typedef TShape Shape;
    BOOST_CONCEPT_ASSERT(( CDigitalOrientedShape<Shape>));

    typedef TSampler Sampler;
    typedef typename Sampler::Point Point;
    typedef typename Sampler::Domain::Vector Vector;
    typedef typename Sampler::Point::Coordinate Coordinate;
    

  
    OutputSensitiveConvexHull(const Shape &aShape,
                              Sampler &aSampler): 
      myShape(aShape), mySampler(aSampler)
    {
    }

    /**
     * Destructor.
     */
    ~OutputSensitiveConvexHull();

    // ----------------------- Interface --------------------------------------
  public:


    /** 
     * Find a first point in the shape using the point sampler.
     * 
     * @param found true if a point has been found.
     * @param nbMax maximal number of trials.
     * 
     * @return a Point contained in the shape (it found is true), a
     * default point instance otherwise.
     */
    Point findFirstPoint(bool &found, const unsigned int nbMax = 10000);


    /** 
     * Compute the next point on the CVXhull
     * 
     * @param previousPoint the previous point
     * 
     * @return the successor.
     */
    Point nextCVXPoint(const Point & previousPoint);
 

    
    /** 
     * Digital Ray shooting. Finds the closest point on the digital
     *    ray with direction Pminus1 and origin Piminus2, which is on
     *    the same side of the shape as Pminus2.
     *
     *  @pre points coordinates are such that the origin is inside the shape.
     *
     * 
     * @param Piminus1 
     * @param Piminus2 
     */
    Point digitalRayShooting(const Vector &Piminus1,
                             const Point &Piminus2) const;
    
    /** 
     * Recursive ray shooting. Finds the point Pi such that we have:
     *    OPi = lambda Piminusone + Piminus2
     *    Pi is on the same side of the shape as Piminus2
     *    lambda is in [minLambda,maxLambda]
     *
     *  @pre points coordinates are such that the origin is inside the shape.
     * 
     * @param Pi 
     * @param Piminus1 
     * @param Piminus2 
     * @param minLambda 
     * @param maxLambda 
     */
    void recursiveDigitalRayShooting(Point &Pi,
                                     const Vector &Piminus1,
                                     const DGtal::Orientation Orientation,
                                     const Coordinate intervalSize) const;

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

    ///Const reference on the input shape
    const Shape & myShape;

    ///Const reference on the input shape
    Sampler & mySampler;

    
    // ------------------------- Private Datas --------------------------------
  private:

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    OutputSensitiveConvexHull();

  private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    OutputSensitiveConvexHull ( const OutputSensitiveConvexHull & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    OutputSensitiveConvexHull & operator= ( const OutputSensitiveConvexHull & other );

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class OutputSensitiveConvexHull


  /**
   * Overloads 'operator<<' for displaying objects of class 'OutputSensitiveConvexHull'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'OutputSensitiveConvexHull' to write.
   * @return the output stream after the writing.
   */
  template <typename Sh, typename Sa>
  std::ostream&
  operator<< ( std::ostream & out, const OutputSensitiveConvexHull<Sh,Sa> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/shapes/shapesGeometry/OutputSensitiveConvexHull.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined OutputSensitiveConvexHull_h

#undef OutputSensitiveConvexHull_RECURSES
#endif // else defined(OutputSensitiveConvexHull_RECURSES)
