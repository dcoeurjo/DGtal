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

    ChamferNorm2D<Z2i::Space>::LessThanAngular lthan;
  ChamferNorm2D<Z2i::Space>::LessOrEqThanAngular lqthan;
  
  ChamferNorm2D<Space>::Directions dirs34;
  ChamferNorm2D<Space>::Directions normals34;
  
  //3-4 mask
  dirs34.push_back(Z2i::Vector(0,-1));
  dirs34.push_back(Z2i::Vector(1,-1));
  dirs34.push_back(Z2i::Vector(1,0));
  dirs34.push_back(Z2i::Vector(1,1));
  dirs34.push_back(Z2i::Vector(0,1));
  normals34.push_back(Z2i::Vector(1,-3));
  normals34.push_back(Z2i::Vector(3,-1));
  normals34.push_back(Z2i::Vector(3,1));
  normals34.push_back(Z2i::Vector(1,3));
  
  ChamferNorm2D<Space> aMask(dirs34,normals34);

  
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
  dirs34.push_back(Z2i::Vector(0,-1));
  dirs34.push_back(Z2i::Vector(1,-1));
  dirs34.push_back(Z2i::Vector(1,0));
  dirs34.push_back(Z2i::Vector(1,1));
  dirs34.push_back(Z2i::Vector(0,1));
  normals34.push_back(Z2i::Vector(1,-3));
  normals34.push_back(Z2i::Vector(3,-1));
  normals34.push_back(Z2i::Vector(3,1));
  normals34.push_back(Z2i::Vector(1,3));
  
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
               << mask.getLowerRayIntersection(P,Q,Lmin,Lmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q,Lmin,Lmax, 1) == 4)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==4" << std::endl;
  
  trace.info() << "Intersection "<<P<<" "<<Qb<<"   = "
  << mask.getLowerRayIntersection(P,Qb,Lmin,Lmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Qb,Lmin,Lmax, 1) == -5)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==-5" << std::endl;
  
  trace.info() << "Intersection "<<P<<" "<<Q2<<"   = "
  << mask.getLowerRayIntersection(P,Q2,Lmin,Lmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q2,Lmin,Lmax, 1) <= Lmin[1])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmin" << std::endl;
 
  trace.info() << "Intersection "<<P<<" "<<Q3<<"   = "
  << mask.getLowerRayIntersection(P,Q3,Lmin,Lmax, 1)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(P,Q3,Lmin,Lmax, 1) >= Lmax[1])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmax" << std::endl;
  
  trace.info()<<std::endl;
  
  Point PP(0,0),QQ(3,7),QQb(-3,7), QQ2(-10, 2), QQ3(10, 2);
  Point LLmin(-10,10), LLmax(10,10);
  
  trace.info() << "Intersection "<<PP<<" "<<QQ<<"   = "
  << mask.getLowerRayIntersection(PP,QQ,LLmin,LLmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ,LLmin,LLmax, 0) == 4)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==4" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQb<<"   = "
  << mask.getLowerRayIntersection(PP,QQb,LLmin,LLmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQb,LLmin,LLmax, 0) == -5)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==-5" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQ2<<"   = "
  << mask.getLowerRayIntersection(PP,QQ2,LLmin,LLmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ2,LLmin,LLmax, 0) <= LLmin[0])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmin" << std::endl;
  
  trace.info() << "Intersection "<<PP<<" "<<QQ3<<"   = "
  << mask.getLowerRayIntersection(PP,QQ3,LLmin,LLmax, 0)<<std::endl;
  nbok += ( mask.getLowerRayIntersection(PP,QQ3,LLmin,LLmax, 0) >= LLmax[0])  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "inter==Lmax" << std::endl;
  
  return nbok == nb;
}


bool testShrink()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing shrinking...");
  
  //5-7-11 metic
  typedef ChamferNorm2D<Z2i::Space> Metric;
  Metric::Directions dirs5711;
  Metric::Directions normals5711;
  //5-7-11 mask
  dirs5711.push_back(Z2i::Vector(0,-1));
  dirs5711.push_back(Z2i::Vector(1,-2));
  dirs5711.push_back(Z2i::Vector(1,-1));
  dirs5711.push_back(Z2i::Vector(2,-1));
  dirs5711.push_back(Z2i::Vector(1,0));
  dirs5711.push_back(Z2i::Vector(2,1));
  dirs5711.push_back(Z2i::Vector(1,1));
  dirs5711.push_back(Z2i::Vector(1,2));
  dirs5711.push_back(Z2i::Vector(0,1));
  
  normals5711.push_back(Z2i::Vector(1,-5));
  normals5711.push_back(Z2i::Vector(3,-4));
  normals5711.push_back(Z2i::Vector(4,-3));
  normals5711.push_back(Z2i::Vector(5,-1));
  normals5711.push_back(Z2i::Vector(5,1));
  normals5711.push_back(Z2i::Vector(4,3));
  normals5711.push_back(Z2i::Vector(3,4));
  normals5711.push_back(Z2i::Vector(1,5));
  
  Metric mask5711(dirs5711,normals5711);
  
  //Setting:
  Point P(0,0);
  Point Q(1,-2), QQ(1,2);
  Point Lmin(10,0);
  Point Lmax(10,10);
  Metric::ConstIterator itBeg = mask5711.begin();
  Metric::ConstIterator itEnd = mask5711.end();
  
  Point midPoint, nextMidPoint;
  
  Metric::ConstIterator cone = mask5711.shrinkPSubMask(itBeg, itEnd, P, Q, Lmin, Lmax, 1, midPoint, nextMidPoint);
  trace.info() <<"Shrink returns the cone "<< *cone<<" " <<*(cone+1)<<std::endl;
  trace.info() <<"MidPoint "<< midPoint<<" " <<nextMidPoint<<std::endl<<std::endl;
  nbok += ( midPoint == Point(10,0))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "midPoint = (10,0)" << std::endl;
  nbok += ( nextMidPoint == Point(10,5))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "nextMidPoint = (10,5)" << std::endl;

  
  trace.warning()<<" Shrinking with QQ(1,2)"<<std::endl;
  
  Metric::ConstIterator cone2 = mask5711.shrinkPSubMask(itBeg, itEnd, P, QQ, Lmin, Lmax, 1, midPoint, nextMidPoint);
  trace.info() <<"Shrink returns the cone2 "<< *cone2<<" " <<*(cone2+1)<<std::endl;
  trace.info() <<"MidPoint "<< midPoint<<" " <<nextMidPoint<<std::endl<<std::endl;
  nbok += ( midPoint == Point(10,-5))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "midPoint = (10,0)" << std::endl;
  nbok += ( nextMidPoint == Point(10,0))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "nextMidPoint = (10,5)" << std::endl;

  
  trace.beginBlock("Testing Symmetry");
  Point LLmin(-10,0);
  Point LLmax(-10,10);
  Point QQQ(-1,-2);
  Metric::ConstIterator cone3 = mask5711.shrinkP(itBeg, itEnd, P, QQQ, LLmin, LLmax, 1, midPoint, nextMidPoint);
  trace.info() <<"Shrink returns the cone3 "<< *cone3<<" " <<*(cone3+1)<<std::endl;
  trace.info() <<"MidPoint "<< midPoint<<" " <<nextMidPoint<<std::endl<<std::endl;
  nbok += ( midPoint == Point(-10,0))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "midPoint = (-10,0)" << std::endl;
  nbok += ( nextMidPoint == Point(-10,5))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "nextMidPoint = (-10,5)" << std::endl;
  trace.endBlock();
  
  //horizontal
  trace.beginBlock("Testing H/V symmetry");
  Point LLLmin(0,10);
  Point LLLmax(10,10);
  Point Qhori(-2,1);
  Metric::ConstIterator cone4 = mask5711.shrinkP(itBeg, itEnd, P, Qhori, LLLmin, LLLmax, 0, midPoint, nextMidPoint);
  trace.info() <<"Shrink returns the cone3 "<< *cone4<<" " <<*(cone4+1)<<std::endl;
  trace.info() <<"MidPoint "<< midPoint<<" " <<nextMidPoint<<std::endl<<std::endl;
  nbok += ( *cone4 == *cone)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "horiz==vert" << std::endl;
  nbok += ( midPoint == Point(0,10))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "midPoint = (0,10)" << std::endl;
  nbok += ( nextMidPoint == Point(5,10))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "nextMidPoint = (5,10)" << std::endl;
  
  Point LLLLmin(0,-10);
  Point LLLLmax(10,-10);
  Point QQhori(-2,-1);
  Metric::ConstIterator cone5 = mask5711.shrinkP(itBeg, itEnd, P, QQhori, LLLLmin, LLLLmax, 0, midPoint, nextMidPoint);
  trace.info() <<"Shrink returns the cone3 "<< *cone4<<" " <<*(cone4+1)<<std::endl;
  trace.info() <<"MidPoint "<< midPoint<<" " <<nextMidPoint<<std::endl;
  nbok += ( *cone5 == *cone)  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "horiz==vert" << std::endl;
  nbok += ( midPoint == Point(0,-10))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "midPoint = (0,-10)" << std::endl;
  nbok += ( nextMidPoint == Point(5,-10))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << "nextMidPoint = (5,-10)" << std::endl;
  trace.endBlock();
  
  trace.endBlock();
  return nbok == nb;
}
bool testDoubleShrink()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing Double Shrinking...");
  
  //5-7-11 metic
  typedef ChamferNorm2D<Z2i::Space> Metric;
  Metric::Directions dirs5711;
  Metric::Directions normals5711;
  //5-7-11 mask
  dirs5711.push_back(Z2i::Vector(0,-1));
  dirs5711.push_back(Z2i::Vector(1,-2));
  dirs5711.push_back(Z2i::Vector(1,-1));
  dirs5711.push_back(Z2i::Vector(2,-1));
  dirs5711.push_back(Z2i::Vector(1,0));
  dirs5711.push_back(Z2i::Vector(2,1));
  dirs5711.push_back(Z2i::Vector(1,1));
  dirs5711.push_back(Z2i::Vector(1,2));
  dirs5711.push_back(Z2i::Vector(0,1));
  
  normals5711.push_back(Z2i::Vector(1,-5));
  normals5711.push_back(Z2i::Vector(3,-4));
  normals5711.push_back(Z2i::Vector(4,-3));
  normals5711.push_back(Z2i::Vector(5,-1));
  normals5711.push_back(Z2i::Vector(5,1));
  normals5711.push_back(Z2i::Vector(4,3));
  normals5711.push_back(Z2i::Vector(3,4));
  normals5711.push_back(Z2i::Vector(1,5));
  
  Metric mask5711(dirs5711,normals5711);
  
  //Setting:
  trace.beginBlock("Tessting shrinking using Q");
  Point P(0,0);
  Point Q(1,-2), QQ(1,2);
  Point Lmin(10,-10);
  Point Lmax(10,10);
  Point midPointP,nextMidPointP;
  Point midPointQ,nextMidPointQ;
  
  Metric::ConstIterator itBeg = mask5711.begin();
  Metric::ConstIterator itEnd = mask5711.end();
  
  Metric::ConstIterator cone = mask5711.shrinkP(itBeg, itEnd, P, Q, Lmin, Lmax, 1, midPointP, nextMidPointP);
  trace.info() <<" P - Shrink returns the cone "<< *cone<<" " <<*(cone+1)<<std::endl;
  trace.info() <<" P - MidPoint "<< midPointP<<" " <<nextMidPointP<<std::endl<<std::endl;
  
  Metric::ConstIterator cone2 = mask5711.shrinkP(itBeg, itEnd, Q, P, Lmin, Lmax, 1, midPointQ, nextMidPointQ);
  trace.info() <<" Q - Shrink returns the cone "<< *cone2<<" " <<*(cone2+1)<<std::endl;
  trace.info() <<" Q - MidPoint "<< midPointQ<<" " <<nextMidPointQ<<std::endl<<std::endl;
  trace.info() << "Checking Voro cell" <<std::endl;
  
  double dpmidp = mask5711(P,midPointP);
  double dqmidp = mask5711(Q,midPointP);
  trace.info() << ((dpmidp < dqmidp) ? "MidP closer to P" : "Mid closer to Q") << std::endl;
  double dpnextmidp = mask5711(P,nextMidPointP);
  double dqnextmidp = mask5711(Q,nextMidPointP);
  trace.info() << ((dpnextmidp < dqnextmidp) ? "NextMidP closer to P" : "Mid closer to Q") << std::endl;
  nbok += ( (dpmidp < dqmidp) != (dpnextmidp < dqnextmidp))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink P contains the Voronoi Edge" << std::endl;
  
  
  double dpmidq = mask5711(P,midPointQ);
  double dqmidq = mask5711(Q,midPointQ);
  trace.info() << ((dpmidq < dqmidq) ? "MidQ closer to P" : "Mid closer to Q") << std::endl;
  double dpnextmidq = mask5711(P,nextMidPointQ);
  double dqnextmidq = mask5711(Q,nextMidPointQ);
  trace.info() << ((dpnextmidq < dqnextmidq) ? "NextMidP closer to P" : "Mid closer to Q") << std::endl;
  nbok += ( (dpmidq < dqmidq) != (dpnextmidq < dqnextmidq))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink Q contains the Voronoi Edge" << std::endl;
  trace.endBlock();
  
  //Setting2
  trace.beginBlock("Testing double shrinking on QQ");
  Metric::ConstIterator coneQQ = mask5711.shrinkP(itBeg, itEnd, P, QQ, Lmin, Lmax, 1, midPointP, nextMidPointP);
  trace.info() <<" P - Shrink returns the cone "<< *coneQQ<<" " <<*(coneQQ+1)<<std::endl;
  trace.info() <<" P - MidPoint "<< midPointP<<" " <<nextMidPointP<<std::endl;
  trace.info() <<" P - cone distance to P("<< mask5711(midPointP,P)<<","
  << mask5711(nextMidPointP,P)<<")" <<std::endl;
  trace.info() <<" P - cone distance to QQ("<< mask5711(midPointP,QQ)<<","
  << mask5711(nextMidPointP,QQ)<<")" <<std::endl<<std::endl;
  
  Metric::ConstIterator coneQQ2 = mask5711.shrinkP(itBeg, itEnd, QQ, P, Lmin, Lmax, 1, midPointQ, nextMidPointQ);
  trace.info() <<" QQ - Shrink returns the cone "<< *coneQQ2<<" " <<*(coneQQ2+1)<<std::endl;
  trace.info() <<" QQ - MidPoint "<< midPointQ<<" " <<nextMidPointQ<<std::endl;
  trace.info() <<" QQ - cone distance to QQ("<< mask5711(midPointQ,QQ)<<","
  << mask5711(nextMidPointQ,QQ)<<")" <<std::endl;
  trace.info() <<" QQ - cone distance to P("<< mask5711(midPointQ,P)<<","
  << mask5711(nextMidPointQ,P)<<")" <<std::endl;
  trace.info() << "Checking Voro cell" <<std::endl<<std::endl;
  
  dpmidp = mask5711(P,midPointP);
  dqmidp = mask5711(QQ,midPointP);
  trace.info() << ((dpmidp < dqmidp) ? "MidP closer to P" : "MidP closer to QQ") << std::endl;
  dpnextmidp = mask5711(P,nextMidPointP);
  dqnextmidp = mask5711(QQ,nextMidPointP);
  trace.info() << ((dpnextmidp < dqnextmidp) ? "NextMidP closer to P" : "NextMidP closer to QQ") << std::endl;
  nbok += ( (dpmidp < dqmidp) != (dpnextmidp < dqnextmidp))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink P contains the Voronoi Edge" << std::endl;
  trace.info()<< "Distances : ConeP<->P("<<dpmidp<<","<<dpnextmidp<<")   ConeP<->Q("<<dqmidp<<","<<dqnextmidp<<")"<<std::endl;
  
  
  dpmidq = mask5711(P,midPointQ);
  dqmidq = mask5711(QQ,midPointQ);
  trace.info() << ((dpmidq < dqmidq) ? "MidQ closer to P" : "MidQ closer to QQ") << std::endl;
  dpnextmidq = mask5711(P,nextMidPointQ);
  dqnextmidq = mask5711(QQ,nextMidPointQ);
  trace.info() << ((dpnextmidq < dqnextmidq) ? "NextMidQ closer to P" : "NextMidQ closer to QQ") << std::endl;
  nbok += ( (dpmidq < dqmidq) != (dpnextmidq < dqnextmidq))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink QQ contains the Voronoi Edge" << std::endl;
  trace.info()<< "Distances : ConeQ<->P("<<dpmidq<<","<<dpnextmidq<<")   ConeQ<->QQ("<<dqmidq<<","<<dqnextmidq<<")"<<std::endl;
  trace.endBlock();
  
  
  return nbok == nb;
}


bool testDoubleShrinkHorizontal()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing Double Shrinking Horizontal...");
  
  //5-7-11 metic
  typedef ChamferNorm2D<Z2i::Space> Metric;
  Metric::Directions dirs5711;
  Metric::Directions normals5711;
  //5-7-11 mask
  dirs5711.push_back(Z2i::Vector(0,-1));
  dirs5711.push_back(Z2i::Vector(1,-2));
  dirs5711.push_back(Z2i::Vector(1,-1));
  dirs5711.push_back(Z2i::Vector(2,-1));
  dirs5711.push_back(Z2i::Vector(1,0));
  dirs5711.push_back(Z2i::Vector(2,1));
  dirs5711.push_back(Z2i::Vector(1,1));
  dirs5711.push_back(Z2i::Vector(1,2));
  dirs5711.push_back(Z2i::Vector(0,1));
  
  normals5711.push_back(Z2i::Vector(1,-5));
  normals5711.push_back(Z2i::Vector(3,-4));
  normals5711.push_back(Z2i::Vector(4,-3));
  normals5711.push_back(Z2i::Vector(5,-1));
  normals5711.push_back(Z2i::Vector(5,1));
  normals5711.push_back(Z2i::Vector(4,3));
  normals5711.push_back(Z2i::Vector(3,4));
  normals5711.push_back(Z2i::Vector(1,5));
  
  Metric mask5711(dirs5711,normals5711);

  //Setting:
  trace.beginBlock("Tessting shrinking using Q");
  Point P(0,0);
  Point Q(-2,1), QQ(2,1);
  Point Lmin(-10,10);
  Point Lmax(10,10);
  Point midPointP,nextMidPointP;
  Point midPointQ,nextMidPointQ;

  Metric::ConstIterator itBeg = mask5711.begin();
  Metric::ConstIterator itEnd = mask5711.end();
  
  Metric::ConstIterator cone = mask5711.shrinkP(itBeg, itEnd, P, Q, Lmin, Lmax, 0, midPointP, nextMidPointP);
  trace.info() <<" P - Shrink returns the cone "<< *cone<<" " <<*(cone+1)<<std::endl;
  trace.info() <<" P - MidPoint "<< midPointP<<" " <<nextMidPointP<<std::endl<<std::endl;
  
  Metric::ConstIterator cone2 = mask5711.shrinkP(itBeg, itEnd, Q, P, Lmin, Lmax, 0, midPointQ, nextMidPointQ);
  trace.info() <<" Q - Shrink returns the cone "<< *cone2<<" " <<*(cone2+1)<<std::endl;
  trace.info() <<" Q - MidPoint "<< midPointQ<<" " <<nextMidPointQ<<std::endl<<std::endl;
  trace.info() << "Checking Voro cell" <<std::endl;
  
  double dpmidp = mask5711(P,midPointP);
  double dqmidp = mask5711(Q,midPointP);
  trace.info() << ((dpmidp < dqmidp) ? "MidP closer to P" : "Mid closer to Q") << std::endl;
  double dpnextmidp = mask5711(P,nextMidPointP);
  double dqnextmidp = mask5711(Q,nextMidPointP);
  trace.info() << ((dpnextmidp < dqnextmidp) ? "NextMidP closer to P" : "Mid closer to Q") << std::endl;
  nbok += ( (dpmidp < dqmidp) != (dpnextmidp < dqnextmidp))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink P contains the Voronoi Edge" << std::endl;
  
  
  double dpmidq = mask5711(P,midPointQ);
  double dqmidq = mask5711(Q,midPointQ);
  trace.info() << ((dpmidq < dqmidq) ? "MidQ closer to P" : "Mid closer to Q") << std::endl;
  double dpnextmidq = mask5711(P,nextMidPointQ);
  double dqnextmidq = mask5711(Q,nextMidPointQ);
  trace.info() << ((dpnextmidq < dqnextmidq) ? "NextMidP closer to P" : "Mid closer to Q") << std::endl;
  nbok += ( (dpmidq < dqmidq) != (dpnextmidq < dqnextmidq))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink Q contains the Voronoi Edge" << std::endl;
  trace.endBlock();
  
  //Setting2
  trace.beginBlock("Testing double shrinking on QQ");
  Metric::ConstIterator coneQQ = mask5711.shrinkP(itBeg, itEnd, P, QQ, Lmin, Lmax, 0, midPointP, nextMidPointP);
  trace.info() <<" P - Shrink returns the cone "<< *coneQQ<<" " <<*(coneQQ+1)<<std::endl;
  trace.info() <<" P - MidPoint "<< midPointP<<" " <<nextMidPointP<<std::endl;
  trace.info() <<" P - cone distance to P("<< mask5711(midPointP,P)<<","
               << mask5711(nextMidPointP,P)<<")" <<std::endl;
  trace.info() <<" P - cone distance to QQ("<< mask5711(midPointP,QQ)<<","
               << mask5711(nextMidPointP,QQ)<<")" <<std::endl<<std::endl;
  
  Metric::ConstIterator coneQQ2 = mask5711.shrinkP(itBeg, itEnd, QQ, P, Lmin, Lmax, 0, midPointQ, nextMidPointQ);
  trace.info() <<" QQ - Shrink returns the cone "<< *coneQQ2<<" " <<*(coneQQ2+1)<<std::endl;
  trace.info() <<" QQ - MidPoint "<< midPointQ<<" " <<nextMidPointQ<<std::endl;
  trace.info() <<" QQ - cone distance to QQ("<< mask5711(midPointQ,QQ)<<","
               << mask5711(nextMidPointQ,QQ)<<")" <<std::endl;
  trace.info() <<" QQ - cone distance to P("<< mask5711(midPointQ,P)<<","
                << mask5711(nextMidPointQ,P)<<")" <<std::endl;
  trace.info() << "Checking Voro cell" <<std::endl<<std::endl;
  
  dpmidp = mask5711(P,midPointP);
  dqmidp = mask5711(QQ,midPointP);
  trace.info() << ((dpmidp < dqmidp) ? "MidP closer to P" : "MidP closer to QQ") << std::endl;
  dpnextmidp = mask5711(P,nextMidPointP);
  dqnextmidp = mask5711(QQ,nextMidPointP);
  trace.info() << ((dpnextmidp < dqnextmidp) ? "NextMidP closer to P" : "NextMidP closer to QQ") << std::endl;
  nbok += ( (dpmidp < dqmidp) != (dpnextmidp < dqnextmidp))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink P contains the Voronoi Edge" << std::endl;
  trace.info()<< "Distances : ConeP<->P("<<dpmidp<<","<<dpnextmidp<<")   ConeP<->Q("<<dqmidp<<","<<dqnextmidp<<")"<<std::endl;
  
  
  dpmidq = mask5711(P,midPointQ);
  dqmidq = mask5711(QQ,midPointQ);
  trace.info() << ((dpmidq < dqmidq) ? "MidQ closer to P" : "MidQ closer to QQ") << std::endl;
  dpnextmidq = mask5711(P,nextMidPointQ);
  dqnextmidq = mask5711(QQ,nextMidPointQ);
  trace.info() << ((dpnextmidq < dqnextmidq) ? "NextMidQ closer to P" : "NextMidQ closer to QQ") << std::endl;
  nbok += ( (dpmidq < dqmidq) != (dpnextmidq < dqnextmidq))  ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  << " Shrink QQ contains the Voronoi Edge" << std::endl;
  trace.info()<< "Distances : ConeQ<->P("<<dpmidq<<","<<dpnextmidq<<")   ConeQ<->QQ("<<dqmidq<<","<<dqnextmidq<<")"<<std::endl;
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

  bool res = testChamferSimple()  && checkCMetricConcept() && testBasicMasks() && testIntersection()
      && testShrink()
      && testDoubleShrink()
      && testDoubleShrinkHorizontal(); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
