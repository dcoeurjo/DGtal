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
 * @file testOutputSensitiveCVX.cpp
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/05/10
 *
 * Functions for testing class OutputSensitiveCVX.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/shapes/shapesGeometry/OutputSensitiveConvexHull.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/geometry/volumes/sampling/UniformSampling.h"
#include "DGtal/math/arithmetic/LightSternBrocot.h"
#include "DGtal/helpers/StdDefs.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class OutputSensitiveCVX.
///////////////////////////////////////////////////////////////////////////////
/**
 * Example of a test. To be completed.
 *
 */
bool testOutputSensitiveCVX()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  

  typedef Ball2D< Z2i::Space > MyBall;
  MyBall circle( 128,128, 50 );
 
  Z2i::Domain domain(Z2i::Point(0,0),Z2i::Point(256,256));
  UniformSampling<Z2i::Domain> sampler(domain);

  trace.beginBlock ( "Testing Point probing ..." );
  OutputSensitiveConvexHull<MyBall, UniformSampling<Z2i::Domain> > outputCVX(circle, sampler);

  bool res;
  Z2i::Point orig =  outputCVX.findFirstPoint(res);

  trace.info() << "Find first point = " << orig << std::endl;

  nbok += res ? 1 : 0; 
  nb++;
  trace.endBlock();
  
  trace.beginBlock ( "Testing ray shooting..." );
  Z2i::Vector dir(1,0);
  Z2i::Point pcontour =  outputCVX.digitalRayShooting(dir,Z2i::Point(128,128));
  trace.info() << "Find first point along the x axis on the contour = " << pcontour<< std::endl;
  nbok += (pcontour == Z2i::Point(177,128)) ? 1 : 0; 
  nb++;

  nbok += (circle.orientation(pcontour) == DGtal::INSIDE) ? 1 : 0;
  nb++;
  nbok += (circle.orientation(pcontour + dir) != DGtal::INSIDE) ? 1 : 0;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "true == true" << std::endl;
  
  dir = Z2i::Vector(0,1);
  pcontour =  outputCVX.digitalRayShooting(dir,Z2i::Point(128,128));
  trace.info() << "Find first point along the x axis on the contour = " << pcontour<< std::endl;
  nbok += (pcontour == Z2i::Point(128,177)) ? 1 : 0; 
  nb++;

  nbok += (circle.orientation(pcontour) == DGtal::INSIDE) ? 1 : 0;
  nb++;
  nbok += (circle.orientation(pcontour + dir) != DGtal::INSIDE) ? 1 : 0;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "true == true" << std::endl;
  
  dir = Z2i::Vector(-1,0);
  pcontour =  outputCVX.digitalRayShooting(dir,Z2i::Point(128,128));
  trace.info() << "Find first point along the x axis on the contour = " << pcontour<< std::endl;
  nbok += (pcontour == Z2i::Point(79,128)) ? 1 : 0; 
  nb++;

  nbok += (circle.orientation(pcontour) == DGtal::INSIDE) ? 1 : 0;
  nb++;
  nbok += (circle.orientation(pcontour + dir) != DGtal::INSIDE) ? 1 : 0;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "true == true" << std::endl;
  
  dir = Z2i::Vector(-1,1);
  pcontour =  outputCVX.digitalRayShooting(dir,Z2i::Point(128,128));
  trace.info() << "Find first point along the x axis on the contour = " << pcontour<<std::endl;
  nbok += (pcontour == Z2i::Point(ceil(128 - 25*sqrt(2)),floor(128 + 25*sqrt(2)))) ? 1 : 0; 
  nb++;

  nbok += (circle.orientation(pcontour) == DGtal::INSIDE) ? 1 : 0;
  nb++;
  nbok += (circle.orientation(pcontour + dir) != DGtal::INSIDE) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "true == true" << std::endl;
  
  trace.endBlock();
  



  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "true == true" << std::endl;
  
  return nbok == nb;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class OutputSensitiveCVX" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  bool res = testOutputSensitiveCVX(); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
