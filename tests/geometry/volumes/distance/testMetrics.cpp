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

/**
 * @file testMetrics.cpp
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/08/29
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "DGtalCatch.h"

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

#include "DGtal/geometry/volumes/distance/CSeparableMetric.h"
#include "DGtal/geometry/volumes/distance/CPowerSeparableMetric.h"
#include "DGtal/geometry/volumes/distance/ExactPredicateLpSeparableMetric.h"
#include "DGtal/geometry/volumes/distance/ExactPredicateLpPowerSeparableMetric.h"
#include "DGtal/geometry/volumes/distance/InexactPredicateLpSeparableMetric.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class Metrics.
///////////////////////////////////////////////////////////////////////////////


TEST_CASE("Exact predicates metrics")
{
  Z2i::Point a( 0,0), b(5, 0), bb(5,-10), bbb(5,5),c(10,0), d(3,3);
  Z2i::Point starting( 0, 5), endpoint(10,5);
  
  ExactPredicateLpSeparableMetric<Z2i::Space, 2> metric;

  SECTION("Checking Closest Predicate")
    {
      REQUIRE( metric.closest(a,d,c) == ClosestFIRST);
    }

  SECTION("Checking hiddenBy l2")
    {
      REQUIRE(!metric.hiddenBy(a,b,c,starting,endpoint,0)); 
      REQUIRE(metric.hiddenBy(a,bb,c,starting,endpoint,0)); 
      REQUIRE(!metric.hiddenBy(a,bbb,c,starting,endpoint,0)); 

    }

   
  ExactPredicateLpSeparableMetric<Z2i::Space, 3> metric3;

  SECTION("Checking hiddenBy l3")
    {
      
      REQUIRE(!metric3.hiddenBy(a,b,c,starting,endpoint,0)); 
      REQUIRE(metric3.hiddenBy(a,bb,c,starting,endpoint,0)); 
      REQUIRE(!metric3.hiddenBy(a,bbb,c,starting,endpoint,0)); 
    }
}

TEST_CASE("Inexact Predicate Metrics")
{
  typedef double Value;
  Z2i::Point a( 0,0), b(5, 0), bb(5,-10), bbb(5,5),c(10,0);
  Z2i::Point starting( 0, 5), endpoint(10,5);
      
  InexactPredicateLpSeparableMetric<Z2i::Space, Value> metric (2.1);
  InexactPredicateLpSeparableMetric<Z2i::Space,Value> metric3(3.1);
      
  SECTION("Checking hiddenBy l2.1")
    {
      REQUIRE(!metric.hiddenBy(a,b,c,starting,endpoint,0)); 
      REQUIRE(metric.hiddenBy(a,bb,c,starting,endpoint,0)); 
      REQUIRE(!metric.hiddenBy(a,bbb,c,starting,endpoint,0)); 
    }
      
  SECTION("Checking hiddenBy l3.1")
    {
      REQUIRE(!metric3.hiddenBy(a,b,c,starting,endpoint,0)); 
      REQUIRE(metric3.hiddenBy(a,bb,c,starting,endpoint,0)); 
      REQUIRE(!metric3.hiddenBy(a,bbb,c,starting,endpoint,0)); 
    }
}


TEST_CASE("Exact Power Metrics")
{
  Z2i::Point a( 0,0), bbis(4, 1), b(5,0), bb(5,-10), bbb(5,5),c(10,0);
  Z2i::Point d(5,-6);
  Z2i::Point starting( 0, 5), endpoint(10,5);
  
  typedef ExactPredicateLpPowerSeparableMetric<Z2i::Space, 2> Metric;
  Metric metric;

  SECTION("Closest")
    {
      REQUIRE(metric.closestPower(bbis,a,0,c,0) == DGtal::ClosestFIRST);  
      REQUIRE(metric.closestPower(bbis,a,10,c,35) != DGtal::ClosestFIRST);
      REQUIRE(!metric.hiddenByPower(a,0,b,0,c,0,starting,endpoint,0)); 
      REQUIRE(metric.hiddenByPower(a,0,bb,0,c,0,starting,endpoint,0)); 
      REQUIRE(!metric.hiddenByPower(a,0,bbb,0,c,0,starting,endpoint,0)); 
      REQUIRE(metric.hiddenByPower(a,0,d,0,c,0,starting,endpoint,0)); 
    }
  
  SECTION("Testing Hidden with w!=0")
    {
      REQUIRE(metric.hiddenByPower(a,0,d,30,c,0,starting,endpoint,0)); 
      REQUIRE(metric.hiddenByPower(a,10,d,10,c,10,starting,endpoint,0)); 
    }
}

TEST_CASE("Test Binary search on metrics")
{
  SECTION("all")
    {
      ExactPredicateLpSeparableMetric<Z2i::Space, 1>::RawValue partialA, partialB;
      ExactPredicateLpSeparableMetric<Z2i::Space, 1> metric;
      typedef ExactPredicateLpSeparableMetric<Z2i::Space, 1>::Abscissa Abscissa;
  
      Z2i::Point a(5,5), b(7,10);
  
      partialA = 5;
      partialB = 7;
  
      // (0,9) strict in B
      // distance( (0,9), (5,5) ) = 9
      // distance( (0,9), (7,10)) = 8
      //
      // (0,8) strict in A
      // distance( (0,8), (5,5) ) = 8
      // distance( (0,8), (7,10) ) = 9
  
      Abscissa res = metric.binarySearchHidden(5, 10, partialA, partialB, 0, 15);
      trace.info() << "Abscissa ="<<res<<std::endl;
      REQUIRE(res == 8 );

      Z2i::Point aa(5,5), bb(6,10);
  
      partialA = 5;
      partialB = 5;
  
      // (0,8)  in BB and AA
      // distance( (0,8), (5,5) ) = 8
      // distance( (0,8), (6,10)) = 8
      //
      // (0,7) strict in AA
      // distance( (0,7), (5,5) ) = 7
      // distance( (0,7), (6,10) ) = 9
  
      Abscissa res2 = metric.binarySearchHidden(5, 10, partialA, partialB, 0, 15);
      trace.info() << "Abscissa ="<<res2<<std::endl;
  
      REQUIRE (res2 == 7);
  
      partialA = 0;
      partialB = 105;
  
      // (0,15) strict in AAAA
      // distance( (0,15), (0,5) ) = 10
      // distance( (0,15), (105,10)) = 110
  
      Abscissa res4 = metric.binarySearchHidden(5, 10, partialA, partialB, 0, 15);
      trace.info() << "Abscissa ="<<res4<<std::endl;

      REQUIRE(res4 >= 15);
    }
}


TEST_CASE("Special cases lp")
{
  ExactPredicateLpSeparableMetric<Z2i::Space, 1> metric;
  Z2i::Point a(5,7),b(5,8),bb(6,8),bbb(7,8),c(5,9), bbbb(105,8);
  Z2i::Point starting(4,0), endpoint(4,15);
  
  REQUIRE(!metric.hiddenBy(a,b,c,starting,endpoint,1));
 
  //(a,bb,c)
  REQUIRE(!metric.hiddenBy(a,bb,c,starting,endpoint,1));
 
  
  //(a,bbb,c)
  REQUIRE(metric.hiddenBy(a,bbb,c,starting,endpoint,1));
 
  //(a,bbbb,c) x_abbbb should be > upper
  REQUIRE(metric.hiddenBy(a,bbbb,c,starting,endpoint,1));
}

TEST_CASE("Special cases L2")
{
  
  //Pythagorician triplet to check predicate
  ExactPredicateLpSeparableMetric<Z2i::Space, 2> metric;
  Z2i::Point a(8,5),b(8,8),bb(9,8),bbb(10,8),c(8,11);
  Z2i::Point starting(4,0), endpoint(4,15);

  REQUIRE(!metric.hiddenBy(a,b,c,starting,endpoint,1));
  REQUIRE(!metric.hiddenBy(a,bb,c,starting,endpoint,1));
  REQUIRE(metric.hiddenBy(a,bbb,c,starting,endpoint,1));
}


TEST_CASE("Concepts")
{
  BOOST_CONCEPT_ASSERT(( concepts::CSeparableMetric<ExactPredicateLpSeparableMetric<Z2i::Space, 2> > ));
  BOOST_CONCEPT_ASSERT(( concepts::CPowerSeparableMetric<ExactPredicateLpPowerSeparableMetric<Z2i::Space, 2> > ));
  BOOST_CONCEPT_ASSERT(( concepts::CSeparableMetric<InexactPredicateLpSeparableMetric<Z2i::Space> > ));  
}



/** @ingroup Tests **/
