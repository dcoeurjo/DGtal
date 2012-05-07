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
 * @file ConvexIntegerPolygon.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2012/04/19
 *
 * Header file for module ConvexIntegerPolygon.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(ConvexIntegerPolygon_RECURSES)
#error Recursive header files inclusion detected in ConvexIntegerPolygon.h
#else // defined(ConvexIntegerPolygon_RECURSES)
/** Prevents recursive inclusion of headers. */
#define ConvexIntegerPolygon_RECURSES

#if !defined ConvexIntegerPolygon_h
/** Prevents repeated inclusion of headers. */
#define ConvexIntegerPolygon_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/CSpace.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/math/arithmetic/IntegerComputer.h"
#include "DGtal/math/arithmetic/ClosedIntegerHalfPlane.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class ConvexIntegerPolygon
  /**
     Description of template class 'ConvexIntegerPolygon' <p> \brief
     Aim: Represents a convex polygon in the two-dimensional digital
     plane.

     It is a model of boost::CopyConstructible,
     boost::DefaultConstructible, boost::Assignable. It is also a
     model of boost::Container since it is a std::list of points. It
     is also a model of CDrawableWithBoard2D, and is displayable on a
     Board2D object.

     It contains no more data than a list of points except mutable
     data for intermediate computations.

     It is a backport of <a
     href="https://gforge.liris.cnrs.fr/projects/imagene">ImaGene</a>.

     @tparam TSpace an arbitrary 2-dimensional model of CSpace.
     @tparam TSequence a model of boost::Sequence whose elements are points (TSpace::Point). Default is list of points.
   */
  template < typename TSpace, 
             typename TSequence = std::list< typename TSpace::Point > >
  class ConvexIntegerPolygon 
    : public TSequence
  {
    BOOST_CONCEPT_ASSERT(( CSpace< TSpace > ));
    BOOST_STATIC_ASSERT(( TSpace::dimension == 2 ));
    BOOST_CONCEPT_ASSERT(( boost::Sequence< TSequence > ));

  public:
    typedef ConvexIntegerPolygon<TSpace,TSequence> Self;
    typedef TSequence Base;

    typedef TSpace Space;
    typedef typename Space::Integer Integer;
    typedef typename Space::Point Point;
    typedef typename Space::Vector Vector;
    typedef IntegerComputer<Integer> MyIntegerComputer;
    typedef HyperRectDomain< Space > Domain; 
    typedef ClosedIntegerHalfPlane< Space > HalfSpace;

    typedef typename Base::value_type Value;
    typedef typename Base::iterator Iterator;
    typedef typename Base::const_iterator ConstIterator;
    typedef typename std::size_t Size;
    typedef std::pair<Size,Size> SizeCouple;

    // The sequence must contain points.
    BOOST_STATIC_ASSERT
    (( ConceptUtils::SameType< Value, Point >::value ));
    
    // Point2I and Point should be the same type.
    typedef typename MyIntegerComputer::Point2I Point2I;
    typedef typename MyIntegerComputer::Vector2I Vector2I;
    typedef typename MyIntegerComputer::Point3I Point3I;
    typedef typename MyIntegerComputer::Vector3I Vector3I;
    BOOST_STATIC_ASSERT(( ConceptUtils::SameType< Point2I, Point >::value ));
    BOOST_STATIC_ASSERT(( ConceptUtils::SameType< Vector2I, Vector >::value ));

  public:
    using Base::size;
    using Base::empty;
    using Base::clear;
    using Base::insert;
    using Base::erase;
    using Base::front;
    using Base::back;
    using Base::push_front;
    using Base::push_back;
    using Base::begin;
    using Base::end;
    using Base::rbegin;
    using Base::rend;

  public:
    // ----------------------- Standard services ------------------------------
  public:

    /**
       Destructor.
     */
    ~ConvexIntegerPolygon();

    /**
     * Constructor.
     */
    ConvexIntegerPolygon();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    ConvexIntegerPolygon ( const Base & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    Self & operator= ( const Base & other );

    /**
       @return the bounding domain of this polygon, i.e. the smallest
       bounding box containing all the points of this polygon.
    */
    Domain boundingBoxDomain() const;

    /**
     * Removes (duplicate) consecutive vertices.
     */
    void purge();

    /**
     * Inserts the point K to the convex polygon before position "pos".
     * @param pos any iterator
     * @param K the point to add
     * @return an iterator on the newly created element.
     */
    Iterator insertBefore( const Iterator & pos, const Point & K );

    /**
       adds the point K to the end of the polygon.
       @param K the point to add
     */
    void pushBack( const Point & K );

    /**
     * @return 2*area of polygon.
     */
    const Integer & twiceArea() const;

    /**
     * if the area of this polygon is not 0, computes centroid, else,
     * computes the middle of the straight line segment.
     *
     * The centroid is a 2D rational point but it is represented as a
     * 3D integer point (a/d,c/d) corresponds to (a,b,d).
     *
     * @return the centroid. The centroid is \b not in reduced form.
     *
     * @see centroid( const Integer & ) const
     */
    Point3I centroid() const;
    
    /**
       This form is faster than centroid if you have already computed the area.

       if \e area is not 0, computes centroid, else, computes the middle
       of the straight line segment.
     
       The centroid is a 2D rational point but it is represented as a
       3D integer point (a/d,c/d) corresponds to (a,b,d).

       @param twice_area the area*2 of this polygon.

       @see centroid() const
     */
    Point3I centroid( const Integer & twice_area ) const;

    // ----------------------- halfspace services -------------------------------
  public:

    /**
       Given some half-plane \a hs, finds the vertices of this polygon
       that borders this half-plane.

       Complexity is O(n), where n is size().

       @param it_next_is_outside (returns) either the vertex that is
       in \a hs and whose successor is not in \a hs, or end() if none
       exists.

       @param it_next_is_inside (returns) either the vertex that is not
       in \a hs and whose successor is in \a hs, or end() if none
       exists.

       @return the couple (number of vertices that are in \a hs, number of vertices).
     */
    SizeCouple findCut( Iterator & it_next_is_outside, Iterator & it_next_is_inside, 
                        const HalfSpace & hs );

    /**
       Cuts the convex polygon with the given half-space constraint.
       
       @param hs any half-space constraint.
       @return 'true' if the polygon was modified, 'false' otherwise.
     */
    bool cut( const HalfSpace & hs );

    /**
       Computes the constraint of the form N.P<=c whose supporting
       line passes through point *it and *(it+1), such that the other
       points of the polygon are inside.

       @param it an iterator on a point of this polygon.
       @return the corresponding half-space.
     */
    HalfSpace halfSpace( ConstIterator it ) const;

    /**
       Computes the constraint of the form N.P<=c whose supporting
       line passes through A and B such that the point \a inP
       satisfies the constraint.
       
       @param A any point.
       @param B any point different from A.
       @param inP any point not on the straight line (AB).
       @return  the corresponding half-space.
     */
    HalfSpace halfSpace( const Point & A, const Point & B, const Point & inP ) const;


    /**
       Computes the set \a aSet all the digital points that belongs to this polygon.

       @param aSet (returns) the set that contains as output all the
       digital points of this polygon.

       @todo this method is for now not efficient.
    */
    template <typename DigitalSet>
    void getIncludedDigitalPoints( DigitalSet & aSet ) const;

    // ----------------------- Helper methods ----------------------------------
    
    /**
       Given a point \a inPt on the boundary of \a hs1, computes the
       closest integer points along the boundary of \a hs1 that are
       separated by \a hs2. Either the intersection is exact and the
       returned points lies at this intersection, or \a inPt
       designates the point that satisfies \a hs2 while \a outPt does
       not satisfy \a hs2. The two points are then separated by the
       direction vector of the half-space.

       @param v (returns) the Bezout vector of the direction vector
       between \a inPt and \a outPt.

       @param inPt (in/out) as input, a point on \a hs1, as output, a
       point on \a hs1 satisfying \a hs2. @pre \a inPt must belong to \a hs1.

       @param outPt (returns) a point on \a hs1 not satisfying \a hs2.

       @return 'true' iff the intersection oh \a hs1 and \a hs2 is
       exact. In this case, outPt is equal to inPt and is at the
       intersection of the two half-space boundaries.
     */
    bool getFirstPointsOfHull( Vector & v, 
                               Point & inPt, // must belong to hs1.
                               Point & outPt,
                               const HalfSpace & hs1,
                               const HalfSpace & hs2 ) const;

    /**
       Computes the border of the upper and of the lower convex hull
       from the starting points inPts[0] (up) and outPts[0]
       down, along the constraint N2.p <= c2 while the vertices
       satisfy the constraint N3.p <= c3. The vertices of the two
       borders are stored at the end of inPts and outPts.
       
       @param inPts (in, out) as input, contains the first point, as
       output the sequence of points satisfying \a hs2 and \a hs3.
       
       @param outPts (in, out) as input, contains the first point, as
       output the sequence of points not satisfying \a hs2 and satisfying
       \a hs3.
       
       @param BV the Bezout vector of the vector between inPts[ 0 ] and outPts[ 0 ].
       
       @param hs2 the half-space that is approached by the two sequences of points.
       @param hs3 the limiting half-space which defines the bounds of
       the approximation.
    */
    void getAllPointsOfHull( std::vector<Point> & inPts,
                             std::vector<Point> & outPts,
                             const Vector & BV, 
                             const HalfSpace & hs2,
                             const HalfSpace & hs3 ) const;

    /**
       Compute the convex hull of grid points satisfying the
       constraints N1.P<=c1, N2.P<=c2 and N3.P>=c3.
       
       N2.P<=c2 corresponds to the cut two parts of computation: from
       constraint 1 to constraint 3 and from constraint 3 to
       constraint 1.
       
       The computed vertices are outputed with the output iterator [itOut].

       @param pointRefC1 and pointRefC3 corresponds to grid point lying on
       the supporting lines of C1 and of C3 resp.
       
       @param pos corresponds to an iterator in the list of vertices
       of the convex, to add the next new vertices
       
       NB: the method also computes grid point satisfying N1.P<=c1 and
       N3.P>=c3 but not satisfying N2.P<=c2. The algorithm uses
       these points that's why they appear in the code.
    */
    template <typename OutputIterator>
    OutputIterator computeConvexHullBorder( OutputIterator itOut,
                                            const Point & pointRefC1, 
                                            const Point & pointRefC3,
                                            const HalfSpace & hs1,
                                            const HalfSpace & hs2,
                                            const HalfSpace & hs3 ) const;

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

    /**
     * @return the style name used for drawing this object.
     */
    std::string className() const;

    // ------------------------- Protected Datas ------------------------------
  private:
    // ------------------------- Private Datas --------------------------------
  private:
    /// A utility object to perform computation on integers. Need not
    /// to be copied when cloning this object. Avoids many dynamic
    /// allocations when using big integers.
    mutable MyIntegerComputer _ic;
    mutable Integer _a, _b, _c, _c1, _c3, _den, _g, _fl, _ce;
    mutable Point _A, _B, _A1, _B1, _A2, _B2;
    mutable Vector _N, _DV, _u, _v;
    mutable std::vector<Point> _inPts, _outPts;

    // ------------------------- Hidden services ------------------------------
  protected:

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class ConvexIntegerPolygon


  /**
   * Overloads 'operator<<' for displaying objects of class 'ConvexIntegerPolygon'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'ConvexIntegerPolygon' to write.
   * @return the output stream after the writing.
   */
  template <typename TSpace, typename TSequence>
  std::ostream&
  operator<< ( std::ostream & out, 
               const ConvexIntegerPolygon<TSpace,TSequence> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/math/arithmetic/ConvexIntegerPolygon.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ConvexIntegerPolygon_h

#undef ConvexIntegerPolygon_RECURSES
#endif // else defined(ConvexIntegerPolygon_RECURSES)
