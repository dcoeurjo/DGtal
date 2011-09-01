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
 * @file DigitalSurface.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2011/09/01
 *
 * Header file for module DigitalSurface.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(DigitalSurface_RECURSES)
#error Recursive header files inclusion detected in DigitalSurface.h
#else // defined(DigitalSurface_RECURSES)
/** Prevents recursive inclusion of headers. */
#define DigitalSurface_RECURSES

#if !defined DigitalSurface_h
/** Prevents repeated inclusion of headers. */
#define DigitalSurface_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/base/CowPtr.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class DigitalSurface
  /**
     Description of template class 'DigitalSurface' <p>

     \brief Aim: Represents a set of n-1-cells in a nD space, together
     with adjacency relation between these cells.

     Proxy class to a DigitalSurfaceContainer.

     @tparam TDigitalSurfaceContainer any model of
     CDigitalSurfaceContainer: the concrete representation chosen for
     the digital surface.
   */
  template <typename TDigitalSurfaceContainer>
  class DigitalSurface
  {
    // ----------------------- types ------------------------------
  public:
    typedef DigitalSurface<TDigitalSurfaceContainer> Self;
    typedef TDigitalSurfaceContainer DigitalSurfaceContainer;
    typedef typename DigitalSurfaceContainer::KSpace KSpace;
    typedef typename DigitalSurfaceContainer::Cell Cell;
    typedef typename DigitalSurfaceContainer::SCell SCell;
    typedef typename DigitalSurfaceContainer::Surfel Surfel;
    typedef typename DigitalSurfaceContainer::SurfelConstIterator ConstIterator;

    // ----------------------- Standard services ------------------------------
  public:
    
    /**
     * Destructor.
     */
    ~DigitalSurface();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    DigitalSurface ( const DigitalSurface & other );

    /**
       Copy constructor from container.
       @param container the container to copy.
    */
    DigitalSurface( const DigitalSurfaceContainer & container );

    /**
       Constructor from pointer on a dynamically allocated container.
       @param containerPtr the pointer to acquire.
    */
    DigitalSurface( DigitalSurfaceContainer* containerPtr );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    DigitalSurface & operator= ( const DigitalSurface & other );

    // ----------------------- Services --------------------------------------
  public:

    /**
       @return a const reference to the stored container.
    */
    const DigitalSurfaceContainer & container() const;

    /**
       @return a reference to the container (may be duplicated if
       several DigitalSurface points on the same).
    */
    DigitalSurfaceContainer & container();

    /**
       @return a ConstIterator on the first surfel in the container.
    */
    ConstIterator begin() const;

    /**
       @return a ConstIterator after the last surfel in the container.
    */
    ConstIterator end() const;

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

    /// a smart pointer on the container.
    CowPtr<DigitalSurfaceContainer> myContainer;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    DigitalSurface();

  private:


    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class DigitalSurface


  /**
   * Overloads 'operator<<' for displaying objects of class 'DigitalSurface'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'DigitalSurface' to write.
   * @return the output stream after the writing.
   */
  template <typename TDigitalSurfaceContainer>
  std::ostream&
  operator<< ( std::ostream & out, const DigitalSurface<T> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/topology/DigitalSurface.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DigitalSurface_h

#undef DigitalSurface_RECURSES
#endif // else defined(DigitalSurface_RECURSES)
