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

    typedef typename Space::Vector Vector;
    typedef typename Space::RealVector RealVector;
    typedef typename Space::Point Point;

    typedef std::vector< Vector > Directions;

    typedef typename Directions::const_iterator ConstIterator;

    typedef typename Space::Integer Value;

    
    
    
    ChamferNorm2D(const unsigned int aN);

    
    ChamferNorm2D(const Directions &aDirectionSet,
                  const Directions &aNormalDirectionSet);
    
    
    /**
     * Destructor.
     */
    ~ChamferNorm2D();


    
    struct LessThanAngular
    {
      bool operator() (const Vector& a, const Vector& b) const
      {
        //   std::cout<<"comparing "<<a<<"    "<<b<<" get = " << (int)(( a[0]*b[1] - a[1]*b[0] ) > 0)<<std::endl;
        return  (( a[0]*b[1] - a[1]*b[0] ) > 0);
      }
    };
    
  
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
     * @param aDirection the direction to probe
     * @param aBegin the begin of the range in which the search is
     * performed.
     * @param aend the end of the range in which the search is
     * performed.
     *
     * @return a const iterator to the cone the direction belongs to.
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
     * Return the canonical ray for a given ray
     *
     * @return Vector (|aRay[0]|,aRay)
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

    
    Value local(const Point &P, const Vector &aDir) const
    {
      return this->operator()(P,P+aDir);
    }
    
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
     * Forbidden by default.
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
