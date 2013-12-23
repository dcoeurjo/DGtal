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

bool testBasicMasks()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing basic chamfer masks...");

  ChamferNorm2D<Space>::Directions dirs34;
  ChamferNorm2D<Space>::Directions normals34;
  
  //3-4 mask
  dirs34.push_back(Vector(1,0));
  normals34.push_back(Vector(3,1));
  dirs34.push_back(Vector(1,1));
  normals34.push_back(Vector(1,3));
  dirs34.push_back(Vector(0,1));
  
  ChamferNorm2D<Space> mask34(dirs34,normals34);
  
  Point orig(0,0);
  Point p(3,0);
  trace.info() <<"Distance "<<p<<" = "<< mask34(orig,p)<<std::endl;;
  trace.info() <<"Cone "<< *(mask34.getCone(p))<< "  "<< *(mask34.getCone(p)+1)<< std::endl;

  Point q(3,1);
  trace.info() <<"Distance "<<q<<" = "<< mask34(orig,q)<<std::endl;;
  trace.info() <<"Cone "<< *(mask34.getCone(q))<< "  "<< *(mask34.getCone(q)+1)<< std::endl;
  
  
  trace.endBlock();
  return nbok == nb;
}

bool testIntersection()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing intersection...");
 
  Point P(0,0),Q(7,3),Qb(7,-3), Q2(2, -10), Q3(2,10);
  Point Lmin(10,-10), Lmax(10,10);
  ChamferNorm2D<Space> mask(1);
  
  trace.info() << "Intersection "<<P<<" "<<Q<<"   = "
               << mask.getLowerRayIntersection(P,Q,Lmin,Lmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q,Lmin,Lmax, 0) == 4)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==4" << std::endl;
  
  trace.info() << "Intersection "<<P<<" "<<Qb<<"   = "
  << mask.getLowerRayIntersection(P,Qb,Lmin,Lmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Qb,Lmin,Lmax, 0) == -5)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==-5" << std::endl;
  
  trace.info() << "Intersection "<<P<<" "<<Q2<<"   = "
  << mask.getLowerRayIntersection(P,Q2,Lmin,Lmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q2,Lmin,Lmax, 0) == Lmin[1])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmin" << std::endl;
 
  trace.info() << "Intersection "<<P<<" "<<Q3<<"   = "
  << mask.getLowerRayIntersection(P,Q3,Lmin,Lmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q3,Lmin,Lmax, 0) == Lmax[1])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmax" << std::endl;
  
  trace.info()<<std::endl;
  
  Point PP(0,0),QQ(3,7),QQb(-3,7), QQ2(-10, 2), QQ3(10, 2);
  Point LLmin(-10,10), LLmax(10,10);
  
  trace.info() << "Intersection "<<PP<<" "<<QQ<<"   = "
  << mask.getLowerRayIntersection(PP,QQ,LLmin,LLmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ,LLmin,LLmax, 1) == 4)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==4" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQb<<"   = "
  << mask.getLowerRayIntersection(PP,QQb,LLmin,LLmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQb,LLmin,LLmax, 1) == -5)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==-5" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQ2<<"   = "
  << mask.getLowerRayIntersection(PP,QQ2,LLmin,LLmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ2,LLmin,LLmax, 1) == LLmin[0])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmin" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQ3<<"   = "
  << mask.getLowerRayIntersection(PP,QQ3,LLmin,LLmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ3,LLmin,LLmax, 1) == LLmax[0])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmax" << std::endl;
  
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

  bool res = testChamferSimple()  && checkCMetricConcept() && testBasicMasks() && testIntersection(); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
