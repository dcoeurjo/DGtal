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
 * @file testChamferDT.cpp
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systemes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2013/12/16
 *
 * Functions for testing class ChamferDT.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <set>
#include <algorithm>
#include "DGtal/base/Common.h"
#include "ConfigTest.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/geometry/volumes/distance/ChamferNorm2D.h"
#include "DGtal/geometry/volumes/distance/CMetric.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace Z2i;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class ChamferDT.
///////////////////////////////////////////////////////////////////////////////

template<typename Abscissa, typename ConstIterator, typename Point>
Abscissa intersectRayL(ConstIterator &ray,
                       const Point &center,
                       const Point &Lmin,
                       const int k)
{
  Abscissa xk;
  if (k != 0)
    return static_cast<Abscissa>(floor((Lmin[0] - center[0] ) / (double)(*ray)[0] * ((*ray)[k])+ center[k]));
  else
    return  static_cast<Abscissa>(floor((Lmin[1] - center[1] ) / (double)(*ray)[1] * ((*ray)[k])+ center[k]));

}

template <typename Abscissa,typename  ConstIterator,typename  Point>
ConstIterator shrinkP(ConstIterator &pBegin,
                      ConstIterator &pEnd,
                      ConstIterator &qBegin,
                      ConstIterator &qEnd,
                      const Point &P,
                      const Point & Q,
                      const Point &Lmin,
                      const Point& Lmax)
{

  //Mid cone
  size_t count = distance (pBegin, pEnd);
  if (count == 1)
    return pBegin;
  ConstIterator mid = pBegin + count /2;

  //on L, we check the vetors of the cone (mid, mid+1);
  Abscissa posCone, posConeNext;
  //double dist;
}


bool checkCMetricConcept()
{
  BOOST_CONCEPT_ASSERT(( CMetric<ChamferNorm2D<Z2i::Space > > ));
  return true;
}


bool testChamferSimple()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing simple chamfer mask localization...");

  ChamferNorm2D<Z2i::Space> aMask(3);
  ChamferNorm2D<Z2i::Space>::LessThanAngular lthan;
  ChamferNorm2D<Z2i::Space>::LessOrEqThanAngular lqthan;

  
  //Testing cone discovery
  Vector d(101,100);
  trace.info() << "Direction "<<d<<std::endl;
  trace.info() << " -> cone "<< *aMask.getCone(d)
  << "  -- " << *(aMask.getCone(d)+1) <<std::endl;
  nbok += ( lthan(d, *(aMask.getCone(d)+1))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(d < cone+1) returns true" << std::endl;
  nbok += (lqthan(*(aMask.getCone(d)),d)) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(cone <= d) returns true" << std::endl;
  
  //testing canonical
  Vector d3(-101,100);
  trace.info() << "Direction "<<d3<<std::endl;
  trace.info() << " -> cone "<< *aMask.getCone(d3)
  << "  -- " << *(aMask.getCone(d3)+1) <<std::endl;
  nbok += ( lthan(aMask.canonicalRay(d3), *(aMask.getCone(d3)+1))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(d < cone+1) returns true" << std::endl;
  nbok += (lqthan(*(aMask.getCone(d3)),aMask.canonicalRay(d3))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(cone <= d) returns true" << std::endl;
 
  
  //aligned
  Vector d2(8,8);
  trace.info() << "Direction "<<d2<<std::endl;
  trace.info() << " -> cone "<< *aMask.getCone(d2)
  << "  -- " << *(aMask.getCone(d2)+1) <<std::endl;
  nbok += ( lthan(d2, *(aMask.getCone(d2)+1))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(d2 < cone+1) returns true" << std::endl;
  nbok += (lqthan(*(aMask.getCone(d2)),d2) ) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "(cone <= d2) returns true" << std::endl;
  

  //Distance test
  Point p(1,1), q(102,101);
  trace.info()<< "Distance between "<<p<<" and "<< q << " = "<< aMask(p,q)<<std::endl;
  trace.info()<< "Distance between "<<q<<" and "<< p << " = "<< aMask(q,p)<<std::endl;
  nbok += ( aMask(p,q) == aMask(q,p))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "d(p,q)==d(q,p)" << std::endl;
  
  
  trace.endBlock();
  return nbok == nb;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class ChamferDT" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  bool res = testChamferSimple()  && checkCMetricConcept(); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
