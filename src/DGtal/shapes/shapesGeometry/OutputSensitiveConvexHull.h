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
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class OutputSensitiveConvexHull
  /**
   * Description of template class 'OutputSensitiveConvexHull' <p>
   * \brief Aim:
   */
  template <typename T>
  class OutputSensitiveConvexHull
  {
    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor.
     */
    ~OutputSensitiveConvexHull();

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
  template <typename T>
  std::ostream&
  operator<< ( std::ostream & out, const OutputSensitiveConvexHull<T> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/shapes/shapesGeometry/OutputSensitiveConvexHull.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined OutputSensitiveConvexHull_h

#undef OutputSensitiveConvexHull_RECURSES
#endif // else defined(OutputSensitiveConvexHull_RECURSES)
