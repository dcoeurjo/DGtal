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
 * @file IntegralInvariantFeatureFromSurfaceAreaEstimator.h
 * @brief Computes the true quantity to each element of a range associated to a parametric shape.
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2015/01/24
 *
 * Header file for module IntegralInvariantFeatureFromSurfaceAreaEstimator.cpp
 *
 * This file is part of the DGtal library.
 *
 * @see testLengthEstimators.cpp, testTrueLocalEstimator.cpp
 */

#if defined(IntegralInvariantFeatureFromSurfaceAreaEstimator_RECURSES)
#error Recursive header files inclusion detected in IntegralInvariantFeatureFromSurfaceAreaEstimator.h
#else // defined(IntegralInvariantFeatureFromSurfaceAreaEstimator_RECURSES)
/** Prevents recursive inclusion of headers. */
#define IntegralInvariantFeatureFromSurfaceAreaEstimator_RECURSES

#if !defined IntegralInvariantFeatureFromSurfaceAreaEstimator_h
/** Prevents repeated inclusion of headers. */
#define IntegralInvariantFeatureFromSurfaceAreaEstimator_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <DGtal/base/Common.h>
#include "DGtal/geometry/surfaces/estimation/CSurfelLocalEstimator.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{
  namespace functors
  {
    /////////////////////////////////////////////////////////////////////////////
    // template class IntegralInvariantFeatureFromSurfaceAreaEstimator
    /**
     * Description of template class 'IntegralInvariantFeatureFromSurfaceAreaEstimator' <p>
     * \brief Aim: Estimate the feature (k1 - k2)^2 using integral
     * invariant estimator from local patch surface area. This
     * functor needs a normal vector estimator (or cache) to evaluate
     * the surface area of the given patch.
     *
     * model of CLocalEstimatorFromSurfelFunctor.
     *
     *
     * @tparam TSurfel type of surfels
     * @tparam TEmbedder type of functors which embed surfel to @f$ \mathbb{R}^3@f$
     * @tparam TNormalVectorEstimator the type of normal vector
     * estimator to consider (e.g. cached version with EstimatorCache class).
     *
     * @see testSphereFitting.cpp
     **/
    template <typename TSurfel,
              typename TEmbedder,
              typename TNormalVectorEstimator>
    class IntegralInvariantFeatureFromSurfaceAreaEstimator
    {
    public:

      ///Surfel type
      typedef TSurfel Surfel;
      ///Surfel embedder
      typedef TEmbedder SCellEmbedder;
      ///Real vector to represent normal vectors
      typedef typename SCellEmbedder::RealPoint RealPoint;
      ///Normal vector estimator type
      typedef TNormalVectorEstimator NormalVectorEstimator;

      //BOOST_CONCEPT_ASSERT(( concepts::CSurfelLocalEstimator<TNormalVectorEstimator> ));
      
      
      ///Quantity return type
      typedef double Quantity;
  

      /**
       * Constructor.
       *
       * @param [in] anEmbedder embedder to map surfel to R^n.
       * @param [in] h gridstep.
       * @param [in] radius integration ball radius (Euclidean)
       * @param [in] anEstimator a normal vector estimator
       */
      IntegralInvariantFeatureFromSurfaceAreaEstimator(ConstAlias<SCellEmbedder> anEmbedder,
                                                             const double h,
                                                             const double radius,
                                                             ConstAlias<NormalVectorEstimator> anEstimator):
        myEmbedder(&anEmbedder), myH2(h*h), myRadius(radius), myNormalEstimatorCache(&anEstimator), myArea(0.0)
      {
        myShift = -32.0/(myRadius*myRadius);
        myRatio = 32.0*myH2 / (M_PI*myRadius*myRadius*myRadius*myRadius);
      }
      

      /**
       * Destructor.
       */
      ~IntegralInvariantFeatureFromSurfaceAreaEstimator( )
      { }
      
      /**
       * Add the geometrical embedding of a surfel to the point list
       *
       * @param [in] aSurf a surfel to add
       * @param [in] aDistance of aSurf to the neighborhood boundary
       */
      void pushSurfel(const Surfel & aSurf,
                      const double aDistance)
      {
        BOOST_VERIFY(aDistance==aDistance);
        
        RealPoint elementary;
        Dimension i = myEmbedder->space().sOrthDir ( aSurf );
        elementary[ i ] = myEmbedder->space().sDirect ( aSurf, i ) ? 1 : -1;
        RealPoint estimatedNormal = myNormalEstimatorCache->eval( &aSurf );          

        ASSERT( estimatedNormal.norm() <= 1.0);
        ASSERT( elementary.norm() <= 1.0);

        //trace.info() << estimatedNormal.norm() << " " << elementary.norm()<< "  "<<elementary.dot(-estimatedNormal)<< std::endl;
        
        myArea += elementary.dot(estimatedNormal);
      }

      /**
       * Evaluate the surface area of the local patch.
       *
       * @return the surface area
       */
      Quantity eval() const
      {
        // return (32.0/(myRadius*myRadius))*((std::abs(myArea)*myH2)/(M_PI*myRadius*myRadius)-1);
        return (myShift + myArea*myRatio);
      }
                             
      
      /**
       * Reset the point list.
       *
       */
      void reset()
      {
        myArea = 0.0;
      }


    private:

      ///Alias of the geometrical embedder
      const SCellEmbedder * myEmbedder;

      ///Square of the grid step
      const double myH2;

      ///Ball radius
      const double myRadius;

      ///NormalVectorCache
      const NormalVectorEstimator *myNormalEstimatorCache;

      ///Surface area
      double myArea;

      ///Ratio (internal)
      double myRatio;
      ///Shift (internal)
      double myShift;
      
    }; // end of class IntegralInvariantFeatureFromSurfaceAreaEstimator
  }
} // namespace DGtal


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined IntegralInvariantFeatureFromSurfaceAreaEstimator_h

#undef IntegralInvariantFeatureFromSurfaceAreaEstimator_RECURSES
#endif // else defined(IntegralInvariantFeatureFromSurfaceAreaEstimator_RECURSES)
