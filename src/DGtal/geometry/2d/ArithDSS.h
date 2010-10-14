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
//LICENSE-END
#pragma once

/**
 * @file ArithDSS.h
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2010/07/01
 *
 * Header file for module ArithDSS.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(ArithDSS_RECURSES)
#error Recursive header files inclusion detected in ArithDSS.h
#else // defined(ArithDSS_RECURSES)
/** Prevents recursive inclusion of headers. */
#define ArithDSS_RECURSES

#if !defined ArithDSS_h
/** Prevents repeated inclusion of headers. */
#define ArithDSS_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/PointVector.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

/////////////////////////////////////////////////////////////////////////////
// class ArithDSS
/**
 * Description of class 'ArithDSS' <p>
 * \brief Aim:
 * Recognition of a digital straight segment (DSS)
 * based on the arithmetical algorithm of 
 * Debled and Reveilles (1995)
 */
template <typename Domain2D>
class ArithDSS
{
    // ----------------------- Standard services ------------------------------
public:

		
		typedef typename Domain2D::Coordinate Integer;
		//2D point of a domain
		typedef typename Domain2D::Point Point;
		//2D vector of a domain
		typedef typename Domain2D::Vector Vector;
		
		typedef typename DGtal::PointVector<2,double> PointD;
    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    ArithDSS(){};

    /**
     * Destructor.
     */
    ~ArithDSS(){};

    // ----------------------- Interface --------------------------------------
public:

    /**
   * Projects the point [m] onto the average straight line (ie (mu+nu)/2).
   * @param m any point expressed in the local reference frame (may not be part of the segment).
   * @return the projected point.
   */
    PointD project( const Point & m ) const;

    /**
     * Projects the point [m] onto the straight line whose points have
     * remainder [r].
     *
     * @param m any point expressed in the local reference frame (may not
     * be part of the segment).
     *
     * @param r the remainder (may not be an integer).
     * @return the projected point.
     */
    PointD project( const Point & m, float r ) const;
    
    /**
     * Projects the point [m] onto the straight line going through point [p].
     *
     * @param m any point expressed in the local reference frame (may not
     * be part of the segment).
     *
     * @param p any point expressed in the local reference frame (may not
     * be part of the segment).
     *
     * @return the projected point.
     */
    PointD project( const Point & m, const Point & p ) const;
  

    /**
     * Defined as: norm( project(cp_n) - project(c_n) )
     * @return the projected length of the segment.
     * @see projectRegularly
     */
    double projectedSegmentLength() const;
    

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) ;
     
    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    /**
		 * Tests whether the union between a point 
     * (adding to the front of the DSS 
     * with respect to the scan orientaion) 
		 * and a DSS is a DSS. 
     * Computes the parameters of the new DSS 
     * with the adding point if true.
     * @param aPoint the new pixel (connected to the DSS) 
     * @return 'true' if the union is a DSS, 'false' otherwise.
     */
    bool addFront(const Point & aPoint);

    /**
		 * Remove the first point of a DSS
     * (located at the back with respect to 
     * the scan orientaion)
	   * if the DSS has more than two points
     * @return 'true' the first point is removed, 'false' otherwise.
     */
    bool removeBack();

    /**
		 * Computes the sequence of (connected) points
		 * belonging to the DSL(a,b,mu,omega)
     * between the first and last point of the DSS
     * @return the computed sequence of points.
     */
    std::vector<Point> recover() const;

    // ------------------------- Protected Datas ------------------------------
protected:

	//parameters of the DSS
	Integer myA, myB, myMu, myOmega;

	//leaning points
	Point myUf, myUl, myLf, myLl;

	//first and last point
	Point myF, myL;

    // ------------------------- Private Datas --------------------------------
	
private:

    // ------------------------- Hidden services ------------------------------
protected:


    /**
		 * Computes the norm of the two components
     * of a 2D vector.
		 * @param x and y, two values. 
     * @return the norm of a 2D vector.
     */
    virtual Integer norm(const Integer& x, const Integer& y) const = 0;

    /**
		 * Returns the 2D vector 
		 * starting at a point of remainder 0
		 * and pointing at a point of remainder omega
     * @return the 2D vector.
     */
    virtual Vector vectorFrom0ToOmega() const = 0;

    /**
		 * Returns the point 
		 * that follows a given point in the DSS
		 * @param aPoint, a given point of the DSS. 
     * @return the next point.
     */
    virtual Point next(const Point& aPoint) const = 0;

private:

    /**
     * Copy constructor.
     * @param other the object to clone.
     * Forbidden by default.
     */
    ArithDSS ( const ArithDSS & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    ArithDSS & operator= ( const ArithDSS & other );

    // ------------------------- Internals ------------------------------------
private:

   /** 
     * Default Style Functor for selfDraw methods
     * 
     * @param aBoard 
     */

    struct SelfDrawStyle
    {
      SelfDrawStyle(LibBoard::Board & aBoard) 
      {
				aBoard.setFillColor(LibBoard::Color::None);
				aBoard.setPenColor(LibBoard::Color::Red);
      }
    };
    
 public:
    /**
     * Draw the object on a LiBoard board
     * @param board the output board where the object is drawn.
     * @tparam Functor a Functor to specialize the Board style
     */
    template<typename Functor>
      void selfDraw( LibBoard::Board & board ) const;
    
    /**
     * Draw the object on a LiBoard board
     * @param board the output board where the object is drawn.
     * @tparam Functor a Functor to specialize the Board style
     */
    void selfDraw( LibBoard::Board & board ) const
      {
				selfDraw<SelfDrawStyle>(board);
      }


}; // end of class ArithDSS


/**
 * Overloads 'operator<<' for displaying objects of class 'ArithDSS'.
 * @param out the output stream where the object is written.
 * @param object the object of class 'ArithDSS' to write.
 * @return the output stream after the writing.
 */
template<typename Domain2D>
std::ostream&
operator<< ( std::ostream & out,  ArithDSS<Domain2D> & object )
  {
      object.selfDisplay( out);
      return out;
    }


} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions/methods if necessary.
#if defined(INLINE)
#include "DGtal/geometry/2d/ArithDSS.ih"
#endif

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ArithDSS_h

#undef ArithDSS_RECURSES
#endif // else defined(ArithDSS_RECURSES)
