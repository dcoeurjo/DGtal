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
 * @file ChamferNorm2D.h
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systemes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2013/12/18
 *
 * Header file for module ChamferNorm2D.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(ChamferNorm2D_RECURSES)
#error Recursive header files inclusion detected in ChamferNorm2D.h
#else // defined(ChamferNorm2D_RECURSES)
/** Prevents recursive inclusion of headers. */
#define ChamferNorm2D_RECURSES

#if !defined ChamferNorm2D_h
/** Prevents repeated inclusion of headers. */
#define ChamferNorm2D_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <vector>
#include <algorithm>
#include "DGtal/base/Common.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class ChamferNorm2D
  /**
   * Description of template class 'ChamferNorm2D' <p>
   * \brief Aim:
   */
  template <typename TSpace>
  class ChamferNorm2D
  {
    // ----------------------- Standard services ------------------------------
  public:


    BOOST_STATIC_ASSERT( TSpace::dimension == 2 );

    //Underlying Space
    typedef TSpace Space;

    //Type for vectors
    typedef typename Space::Vector Vector;
    //Type for points
    typedef typename Space::Point Point;
    
    //Container for set of directions
    typedef std::vector< Vector > Directions;
    typedef typename Directions::const_iterator ConstIterator;

    //ValueType for being model of CMetric
    typedef typename Space::Integer Value;
    
    //Vector components type
    typedef typename Vector::Component Abscissa;
    
    
    /**
     * Constructor from a number N.
     * Will generate a partia mask with @a aN directions.
     *
     */
    ChamferNorm2D(const unsigned int aN);

    /** Constructor from direction and noraml vector sets.
     *
     * @param aDirectionSet the set of vectors (1st quadrant) of the chamfer norm mask.
     * @param aNormalDirectionSet for each cone, the associated normal vector.
     */
    ChamferNorm2D(const Directions &aDirectionSet,
                  const Directions &aNormalDirectionSet);
    
    
    /**
     * Destructor.
     */
    ~ChamferNorm2D();


    /****
     * @brief Vector comparator (1st quadrant)
     */
    struct LessThanAngular
    {
      bool operator() (const Vector& a, const Vector& b) const
      {
        return  (( a[0]*b[1] - a[1]*b[0] ) > 0);
      }
    };
    
  
    /****
     * @brief Vector comparator (1st quadrant)
     */
    struct LessOrEqThanAngular
    {
      bool operator() (const Vector& a, const Vector& b) const
      {
        return  (( a[0]*b[1] - a[1]*b[0] ) >= 0);
      }
    };


    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Returns the cone associated to a direction (iterator @a it) in
     * the range [@a aBegin, @a eEnd).
     *
     * The cone is given by [it, it+1):
     *
     *    (*it) <= aDirection < (*(it+1))
     * (For the angular comparator).
     *
     * @pre We have *aBegin <= aDirection < *aEnd
     *
     * @param [in] aDirection the direction to probe
     * @param [in] aBegin the begin of the range in which the search is
     * performed.
     * @param [in] aEnd the end of the range in which the search is
     * performed.
     *
     * @return a ConstIterator to the cone the direction belongs to.
     */
    ConstIterator getCone(const Vector &aDirection,
                          ConstIterator aBegin ,
                          ConstIterator aEnd ) const;

  
    /**
     * Returns the cone associated to a direction (iterator @a it) in
     * the whole mask.
     *
     * The cone is given by [it, it+1):
     *
     *    (*it) <= aDirection < (*(it+1))
     * (For the angular comparator).
     *
     * @pre We have *aBegin <= aDirection < *aEnd
     *
     * @param aDirection the direction to probe
     *
     * @return a const iterator to the cone the direction belongs to.
     */
    ConstIterator getCone(const Vector &aDirection) const
    {
      return getCone(aDirection, myDirections.begin(), myDirections.end());
    }

    /**
     *
     * @param aCone the input cone
     * @returns the normal associated with a cone (aCone, aCone+1)
     */
    Vector getNormalFromCone(ConstIterator aCone) const;
    
  
    /**
     * Return the canonical ray for a given ray.
     *
     * This method construct a vector in the first quadrant
     * corresponding to @a aRay.
     *
     * @return Vector |aRay[i]|
     **/
    Vector canonicalRay(const Vector &aRay) const;
    
    // ----------------------- CMetric concept -----------------------------------

    /**
     * Returns the distance for the chamfer norm between P and Q.
     *
     * @param P a point
     * @param Q a point
     *
     * @return the distance between P and Q.
     */
    Value operator()(const Point &P, const Point &Q) const;

    
    /**
     * Returns the distance for the chamfer norm between P and P+aDir.
     *
     * @param P a point
     * @param aDir a direction
     *
     * @return the distance between P and P+aDir.
     */
    Value local(const Point &P, const Vector &aDir) const
    {
      return this->operator()(P,P+aDir);
    }
    
    /**
     * Given an origin and two points, this method decides which one
     * is closest to the origin. This method should be faster than
     * comparing distance values.
     *
     * @param origin the origin
     * @param first  the first point
     * @param second the second point
     *
     * @return a Closest enum: FIRST, SECOND or BOTH.
     */
    DGtal::Closest closest(const Point &origin,
                           const Point &first,
                           const Point &second) const
    {
     Value a = this->operator()(origin,first),b = this->operator()(origin,second);
      if (a<b)
        return ClosestFIRST;
      else
        if (a>b)
          return ClosestSECOND;
        else
          return ClosestBOTH;
    }
    
    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    ChamferNorm2D ( const ChamferNorm2D & other )
    {
      this->myDirections = other.myDirections;
      this->myNormals = other.myNormals;
    }
    
    
    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    ChamferNorm2D<Space> & operator= ( const ChamferNorm2D<Space> & other )
    {
      this->myDirections = other.myDirections;
      this->myNormals = other.myNormals;
      return *this;
    }

    // ----------------------- CSeparableMetric concept -----------------------------------

    /**
     * Compute the intersection between (@a aP,@a aQ) and 
     * (@a Lmin, @a Lmax). More precisely, if we suppose that
     *
     * @pre  We have @f$ Lmin[aDimension] < Lmax[aDimension]@f$
     * @pre We have  @f$ Lmin[(aDimension+1)%2] == Lmax[(aDimension+1)%2]@f$
     * @pre We have  @f$ (aP,aQ) \cap (Lmin,Lmax) @f$
     *
     * this method returns the lower rounding abscissa of the rational
     * intersection point.
     *
     * @param aP first extremity point
     * @param aQ second extremity point
     * @apram Lmin first extremity point of the L segment
     * @apram Lmin first extremity point of the L segment
     * @return the lowest rounding intersection point.
     **/
    Abscissa getLowerRayIntersection(const Vector &aP, const Vector &aQ,
                                     const Point &Lmin, const Point &Lmax,
                                     const Dimension aDimension) const ;
    
    /**
     * Compute the intersection between (@a aP,@a aQ) and
     * (@a Lmin, @a Lmax). More precisely, if we suppose that
     *
     * @pre  We have @f$ Lmin[aDimension] < Lmax[aDimension]@f$
     * @pre  We have @f$ Lmin[(aDimension+1)%2] == Lmax[(aDimension+1)%2]@f$
     * @pre  We have @f$ (aP,aQ) \cap (Lmin,Lmax) @f$
     *
     * this method returns the upper rounding abscissa of the rational
     * intersection point.
     *
     * @param aP first extremity point
     * @param aQ second extremity point
     * @apram Lmin first extremity point of the L segment
     * @apram Lmin first extremity point of the L segment
     * @return the lowest rounding intersection point.
     **/
    Abscissa getUpperRayIntersection(const Vector &aP, const Vector &aQ,
                                     const Point &Lmin, const Point &Lmax,
                                     const Dimension aDimension) const ;
    
    
    
    /**
     * Considering a vertical configuration (aDimension == 1, P[!dimension]<= Lmin[!dimension]),
     * This method returns the cone (ConstIterator, ConstIterator+1) at @a P which contains
     * the Voronoi Edge of P and Q. 
     *
    * @pre  We have aP[aDimension] != aQ[aDimension  (no alignement)
     *
     * This method runs in O(log^2(n)), n being the number of directions in the mask.
     *
     * @param [in] aBegin begin iterator on the current set of directions at P
     * @param [in] aEnd end iterator on the current set of directions at P
     *
     * @param [in] aP center of the ball we are shrinking
     * @param [in] aQ center of the second ball
     * @apram [in]  Lmin first extremity point of the L segment
     * @apram [in] Lmax first extremity point of the L segment
     * @param [in] aDimension direction of the (Lmin,LMax segment)
     * @param [out] midPoint the point, on the segment, corresponding to the intersection
     * of the the first cone direction and the segment
     * @param [out] nextMidPoint the point, on the segment, corresponding to the intersection
     * of the second cone direction and the segment
     * @return the ConstIterator to the cone direction the Voronoi Edge belongs to
     *
     */
    ConstIterator shrinkPSubMask(ConstIterator aBegin,
                                 ConstIterator aEnd,
                                 const Point &aP, const Point &aQ,
                                 const Point &Lmin, const Point &Lmax,
                                 const Dimension aDimension,
                                 Point &midPoint,
                                 Point &nextMidPoint) const ;
    
    
    /**
     *
     *
     *
     * @param [in] aBegin begin iterator on the current set of directions at P
     * @param [in] aEnd end iterator on the current set of directions at P
     *
     * @param [in] aP center of the ball we are shrinking
     * @param [in] aQ center of the second ball
     * @apram [in]  Lmin first extremity point of the L segment
     * @apram [in] Lmax first extremity point of the L segment
     * @param [in] aDimension direction of the (Lmin,LMax segment)
     * @param [out] midPoint the point, on the segment, corresponding to the intersection
     * of the the first cone direction and the segment
     * @param [out] nextMidPoint the point, on the segment, corresponding to the intersection
     * of the second cone direction and the segment
     * @return the ConstIterator to the cone direction the Voronoi Edge belongs to
     *
     */
     ConstIterator shrinkP(ConstIterator aBegin,
                          ConstIterator aEnd,
                          const Point &aP, const Point &aQ,
                          const Point &Lmin, const Point &Lmax,
                          const Dimension aDimension,
                          Point &midPoint,
                          Point &nextMidPoint) const ;
    
    
    
    /**
     * @param u a site
     * @param v a site
     * @param startingPoint starting point of the segment
     * @param endPoint end point of the segment
     * @param dim direction of the straight line
     *
     * @return the abscissa on (startingPoint,endPoint) of the Voronoi cell.
     */
    Abscissa getLowerVoronoiEdgeAbscissa(const Point &u,
                                         const Point &v,
                                         const Point &startingPoint,
                                         const Point &endPoint,
                                         const Dimension dim) const;
    
    
    
    /**
    * @param u a site
    * @param v a site
    * @param w a site
    * @param startingPoint starting point of the segment
    * @param endPoint end point of the segment
    * @param dim direction of the straight line
    *
    * @return true if (u,w) hides v (strictly).
    */
    bool hiddenBy(const Point &u,
                  const Point &v,
                  const Point &w,
                  const Point &startingPoint,
                  const Point &endPoint,
                  const Dimension dim) const;

                                         
                                         
    // ------------------------- Directions iterators ------------------------------
    /**
     * @return the begin iterator to the mask direction set
     */
    ConstIterator begin()
    {
      return myDirections.begin();
    }
    
    /**
     * @return the end iterator to the mask direction set
     */
    ConstIterator end()
    {
      return myDirections.end();
    }
    
    /*
     * @return the mask size
     */
    inline
    unsigned int size() const
    {
      return myDirections.size();
    }
    
    // ------------------------- Other services ------------------------------
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


    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    ChamferNorm2D();

   // ------------------------- Internals ------------------------------------
  private:

    //Mask container
    Directions myDirections;

    //Normal to cone container
    Directions myNormals;

    //Instance of comparator functor
    static LessThanAngular myLessThanAngular;

    BOOST_STATIC_CONSTANT(Abscissa, myInfinity = 32562);
    
    
  }; // end of class ChamferNorm2D


  /**
   * Overloads 'operator<<' for displaying objects of class 'ChamferNorm2D'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'ChamferNorm2D' to write.
   * @return the output stream after the writing.
   */
  template <typename T>
  std::ostream&
  operator<< ( std::ostream & out, const ChamferNorm2D<T> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/volumes/distance//ChamferNorm2D.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined ChamferNorm2D_h

#undef ChamferNorm2D_RECURSES
#endif // else defined(ChamferNorm2D_RECURSES)
