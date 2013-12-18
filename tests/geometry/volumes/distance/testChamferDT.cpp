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
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace Z2i;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class ChamferDT.
///////////////////////////////////////////////////////////////////////////////


template <typename TVector>
struct LessThanAngular
{

  bool operator() (const TVector& a, const TVector& b) const
  {
    return  (( (DGtal::int64_t)a[0]*(DGtal::int64_t)b[1] -
               (DGtal::int64_t)a[1]*(DGtal::int64_t)b[0] ) > 0);
  }
};


bool testContainer()
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  trace.beginBlock ( "Testing std::set container for 2D ChamferDT ..." );

  Vector v(1,1);
  Vector v2(2,1);
  Vector v3(1,2);
  LessThanAngular<Vector> lthan;

  trace.info()<<"v =  "<<v<<std::endl;
  trace.info()<<"v2 =  "<<v2<<std::endl;
  trace.info()<<"v3 =  "<<v3<<std::endl;

  //Few orientation tests
  nbok += lthan(v2,v) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "v2 < v" << std::endl;

  nbok += lthan(v,v3) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "v < v3" << std::endl;

  //Adding some vectors
  typedef std::set<Vector, LessThanAngular<Vector> >  Mask;
  Mask mask;
  for(unsigned int i = 0; i < 10; i++)
    mask.insert( Vector( i, rand() % 10 ));
  //Display
  std::copy( mask.begin(), mask.end(), std::ostream_iterator<Vector>(std::cout, "\n"));


  //Check
  std::set<int> myset;
  myset.insert(10);
  myset.insert(20);
  myset.insert(30);
  myset.insert(40);
  myset.insert(50);

  trace.info()<<std::endl;
  trace.info()<<std::endl;
  trace.info() << "probe = "<<25<<std::endl;
  trace.info() << "Lower bound = " << * (myset.lower_bound(25))<<std::endl;
  trace.info() << "Upper bound = " << * (myset.upper_bound(25))<<std::endl;
  trace.info()<<std::endl;
  trace.info()<<std::endl;

  //get Cone
  Vector probe(102,100);
  trace.info() << "probe = "<<probe<<std::endl;
  trace.info() << "Lower bound = " << * (mask.lower_bound(probe))<<std::endl;
  trace.info() << "Upper bound = " << * (mask.upper_bound(probe))<<std::endl;
  trace.info() << "Lower bound from range = " << * (mask.equal_range(probe).first)<<std::endl;
  trace.info() << "Upper bound from range = " << * (mask.equal_range(probe).second)<<std::endl;
  nbok += lthan(probe, * (mask.upper_bound(probe))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "probe < upper" << std::endl;
  nbok += (!lthan(probe,  * (mask.lower_bound(probe)))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "lower <= probe" << std::endl;

  Vector probe2(15,16);
  trace.info() << "probe = "<<probe2<<std::endl;
  trace.info() << "Lower bound = " << * (mask.lower_bound(probe2))<<std::endl;
  trace.info() << "Upper bound = " << * (mask.upper_bound(probe2))<<std::endl;
  nbok += lthan(probe2, * (mask.upper_bound(probe2))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "probe2 < upper" << std::endl;
  nbok += (!lthan(probe2,  * (mask.lower_bound(probe2)))) ? 1 : 0;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << "lower <= probe2" << std::endl;

  Vector probe3(8,-1);
  trace.info() << "probe = "<<probe3<<std::endl;
  trace.info() << "Lower bound = " << * (mask.lower_bound(probe3))<<std::endl;
  trace.info() << "Upper bound = " << * (mask.upper_bound(probe3))<<std::endl;

  Vector probe4(-1,8);
  trace.info() << "probe = "<<probe4<<std::endl;
  trace.info() << "Lower bound = " << * (mask.lower_bound(probe4))<<std::endl;
  trace.info() << "Upper bound = " << * (mask.upper_bound(probe4))<<std::endl;

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

  bool res = testContainer(); // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
