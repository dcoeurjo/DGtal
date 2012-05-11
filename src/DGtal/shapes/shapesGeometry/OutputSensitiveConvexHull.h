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
   * @tparam TFraction a model of Fractions
   */
  template <typename TShape, typename TSampler, typename TFraction>
  class OutputSensitiveConvexHull
  {
    // ----------------------- Standard services ------------------------------
  public:

    typedef TShape Shape;
    BOOST_CONCEPT_ASSERT(( CDigitalOrientedShape<Shape>));

    typedef TSampler Sampler;
    typedef typename Sampler::Point Point;
    typedef typename Sampler::Point::Coordinate Coordinate;
    

    typedef TFraction Fraction;
    BOOST_CONCEPT_ASSERT(( CPositiveIrreducibleFraction<Fraction> ));

    


    OutputSensitiveConvexHull(const Shape &aShape,
                              const Sampler &aSampler): 
      myShape(aShape), mySampler(aSampler)
    {}

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
     * Given a point inside the shape, the method returns a point
     * belonging to the shape contour (a point 4-adjacent to a
     * background pixel). This method returns the point by a
     * logarithmic search (@a O(log n)
     * where @a n is the distance between @a anInsidePoint and the
     * domain boundary according the direction).
     * 
     * @pre anInsidePoint must be inside the shape.
     * @param aInsidePoint a Point inside the shape
     * @param direction prefered direction using freeman code (default=0).
     * 
     * @return a Point belonging to the contour.
     */
    Point findFirstPointOnContour(const Point &anInsidePoint, 
                                  const DGtal::Dimension dimension = 0) const;


    void recursiveFindOnRay(Point &aPoint,
                            const Coordinate minId,
                            const Coordinate maxId,
                            const DGtal::Dimension dimension) const;
    

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
    const Sampler & mySampler;

    

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
  template <typename Sh, typename Sa, typename F>
  std::ostream&
  operator<< ( std::ostream & out, const OutputSensitiveConvexHull<Sh,Sa,F> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/shapes/shapesGeometry/OutputSensitiveConvexHull.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined OutputSensitiveConvexHull_h

#undef OutputSensitiveConvexHull_RECURSES
#endif // else defined(OutputSensitiveConvexHull_RECURSES)
