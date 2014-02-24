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
 * @file testMetricBalls.cpp
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/11/11
 *
 * Functions for testing class MetricBalls.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

#include "DGtal/geometry/volumes/distance/ExactPredicateLpSeparableMetric.h"
#include "DGtal/geometry/volumes/distance/InexactPredicateLpSeparableMetric.h"

#include "DGtal/geometry/volumes/distance/SeparableMetricAdapter.h"
#include "DGtal/geometry/volumes/distance/ChamferNorm2D.h"


#include "DGtal/io/boards/Board2D.h"
#include "DGtal/kernel/BasicPointPredicates.h"
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class MetricBalls.
///////////////////////////////////////////////////////////////////////////////


template <int norm>
bool testExactMetricBalls(const unsigned int N)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;
  Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(N,N));
  typedef ExactPredicateLpSeparableMetric<Z2i::Space, norm> Metric;
  Z2i::DigitalSet set(domain);

  set.insertNew(Z2i::Point(N/2,N/2));
  typedef NotPointPredicate< Z2i::DigitalSet > NegPred;
  // SetPredicate<Z2i::DigitalSet> setpred(set);
  NegPred predicate( set );

  trace.beginBlock("Exact metrcic"+  boost::lexical_cast<string>( norm ));
  typedef  DistanceTransformation< Z2i::Space, NegPred, Metric> DT;
  Metric metric;
  DT dt( &domain, &predicate, &metric);
  trace.endBlock();


  Board2D board;
  typedef HueShadeColorMap<typename DT::Value,2> Hue;
  board.setUnit ( LibBoard::Board::UCentimeter );
  Display2DFactory::drawImage<Hue> (board, dt,
                                  0.0,
                                  N/2.0*sqrt(2)/2.0);

  std::string title = "image-ball-" +  boost::lexical_cast<string>( norm )+".png" ;
#ifdef WITH_CAIRO
  board.saveCairo(title.c_str(), Board2D::CairoPNG );
#endif
  return nbok == nb;
}

bool testInexactMetricBalls(double norm, const unsigned int N)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;


  Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(N,N));
  typedef InexactPredicateLpSeparableMetric<Z2i::Space> Metric;
  Z2i::DigitalSet set(domain);

  set.insertNew(Z2i::Point(N/2,N/2));
  typedef NotPointPredicate< Z2i::DigitalSet > NegPred;
  // SetPredicate<Z2i::DigitalSet> setpred(set);
  NegPred predicate( set );

  trace.beginBlock("Inex metrcic"+  boost::lexical_cast<string>( norm ));
  typedef  DistanceTransformation< Z2i::Space, NegPred, Metric> DT;
  Metric metric(norm);
  DT dt( &domain, &predicate, &metric);
  trace.endBlock();

  Board2D board;
  typedef HueShadeColorMap< DT::Value,2> Hue;
  board.setUnit ( LibBoard::Board::UCentimeter );
  Display2DFactory::drawImage<Hue> (board, dt,
                                    0.0,
                                    N/2.0*sqrt(2)/2.0);

  std::string title = "image-ball-" +  boost::lexical_cast<string>( norm )+".png" ;
#ifdef WITH_CAIRO
  board.saveCairo(title.c_str(), Board2D::CairoPNG );
#endif
  return nbok == nb;
}


bool testChamfer(const unsigned int N)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;

  Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(N,N));
  Z2i::DigitalSet set(domain);

  set.insertNew(Z2i::Point(N/2,N/2));
  typedef NotPointPredicate< Z2i::DigitalSet > NegPred;
  // SetPredicate<Z2i::DigitalSet> setpred(set);
  NegPred predicate( set );


  //3-4 metic
  typedef ChamferNorm2D<Z2i::Space> Metric;
  Metric::Directions dirs34;
  Metric::Directions normals34;
  //3-4 mask
  dirs34.push_back(Z2i::Vector(0,-1));
  dirs34.push_back(Z2i::Vector(1,-1));
  dirs34.push_back(Z2i::Vector(1,0));
  dirs34.push_back(Z2i::Vector(1,1));

  normals34.push_back(Z2i::Vector(1,-3));
  normals34.push_back(Z2i::Vector(3,-1));
  normals34.push_back(Z2i::Vector(3,1));
  normals34.push_back(Z2i::Vector(1,3));

  Metric mask34(dirs34,normals34);

  typedef SeparableMetricAdapter<Metric> AdaptedMetric;
  AdaptedMetric metric(mask34);


  trace.beginBlock("Chamfer 3-4");
  //DT
  typedef  DistanceTransformation< Z2i::Space, NegPred, AdaptedMetric> DT;
  DT dt( &domain, &predicate, &metric);
  trace.endBlock();

  double dtmax = 0;
  for(DT::ConstRange::ConstIterator it=dt.constRange().begin(),
      itend=dt.constRange().end(); it!= itend;
      ++it)
    if ((*it)>dtmax) dtmax=(*it);



  Board2D board;
  typedef HueShadeColorMap< DT::Value,2> Hue;
  board.setUnit ( LibBoard::Board::UCentimeter );
  Display2DFactory::drawImage<Hue> (board, dt,
                                    0.0,
                                    dtmax);

  std::string title = "image-ball-chamf34.png" ;
#ifdef WITH_CAIRO
  board.saveCairo(title.c_str(), Board2D::CairoPNG );
#endif
  return nbok == nb;
}


bool testChamfer2(const unsigned int N)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;


  Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(N,N));
  Z2i::DigitalSet set(domain);

  set.insertNew(Z2i::Point(N/2,N/2));
  typedef NotPointPredicate< Z2i::DigitalSet > NegPred;
  // SetPredicate<Z2i::DigitalSet> setpred(set);
  NegPred predicate( set );

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

  normals5711.push_back(Z2i::Vector(1,-5));
  normals5711.push_back(Z2i::Vector(3,-4));
  normals5711.push_back(Z2i::Vector(4,-3));
  normals5711.push_back(Z2i::Vector(5,-1));
  normals5711.push_back(Z2i::Vector(5,1));
  normals5711.push_back(Z2i::Vector(4,3));
  normals5711.push_back(Z2i::Vector(3,4));
  normals5711.push_back(Z2i::Vector(1,5));

  Metric mask5711(dirs5711,normals5711);

  typedef SeparableMetricAdapter<Metric> AdaptedMetric;
  AdaptedMetric metric(mask5711);

  //DT
  trace.beginBlock("Chamfer 5-7-11");
  typedef  DistanceTransformation< Z2i::Space, NegPred, AdaptedMetric> DT;
  DT dt( &domain, &predicate, &metric);
  trace.endBlock();

  Board2D board;
  typedef HueShadeColorMap< DT::Value,2> Hue;
  board.setUnit ( LibBoard::Board::UCentimeter );

  double dtmax = 0;
  for(DT::ConstRange::ConstIterator it=dt.constRange().begin(),
      itend=dt.constRange().end(); it!= itend;
      ++it)
    if ((*it)>dtmax) dtmax=(*it);

  Display2DFactory::drawImage<Hue> (board, dt,
                                    0.0,
                                    dtmax);

  std::string title = "image-ball-chamf5711.png" ;
#ifdef WITH_CAIRO
  board.saveCairo(title.c_str(), Board2D::CairoPNG );
#endif
  return nbok == nb;
}


bool testChamfer3(const unsigned int N)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;


  Z2i::Domain domain(Z2i::Point(0,0), Z2i::Point(N,N));
  Z2i::DigitalSet set(domain);

  set.insertNew(Z2i::Point(N/2,N/2));
  typedef NotPointPredicate< Z2i::DigitalSet > NegPred;
  // SetPredicate<Z2i::DigitalSet> setpred(set);
  NegPred predicate( set );

  //Parametrized Chamfer
  typedef ChamferNorm2D<Z2i::Space> Metric;
  Metric mask3( 3 ), mask10(10), mask20(20);
  Metric mask100(100);

  typedef SeparableMetricAdapter<Metric> AdaptedMetric;
  AdaptedMetric metric(mask3);
  AdaptedMetric metric10(mask10);
  AdaptedMetric metric20(mask20);
  AdaptedMetric metric100(mask100);

  //DT
  trace.beginBlock("Chamfer  N=3");
  typedef  DistanceTransformation< Z2i::Space, NegPred, AdaptedMetric> DT;
  DT dt( &domain, &predicate, &metric);
  trace.endBlock();

  trace.beginBlock("Chamfer  N=10");
  DT dt10( &domain, &predicate, &metric10);
  trace.endBlock();

  trace.beginBlock("Chamfer  N=20");
  DT dt20( &domain, &predicate, &metric20);
  trace.endBlock();

  trace.beginBlock("Chamfer  N=100");
  DT dt100( &domain, &predicate, &metric100);
  trace.endBlock();

  Board2D board;
  typedef HueShadeColorMap< DT::Value,2> Hue;
  board.setUnit ( LibBoard::Board::UCentimeter );

  double dtmax = 0;
  for(DT::ConstRange::ConstIterator it=dt100.constRange().begin(),
      itend=dt100.constRange().end(); it!= itend;
      ++it)
    if ((*it)>dtmax) dtmax=(*it);

  Display2DFactory::drawImage<Hue> (board, dt100  ,
                                    0.0,
                                    dtmax);

  std::string title = "image-ball-chamfN.png" ;
#ifdef WITH_CAIRO
  board.saveCairo(title.c_str(), Board2D::CairoPNG );
#endif
  return nbok == nb;
}

///////////////////////////////////////////////////////////////////////////////
// Standard services - public :
int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class MetricBalls" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;
  unsigned int N=64;
  if (argc == 2)
    N = atoi(argv[1]);

  bool res = testExactMetricBalls<2>(N)
    && testExactMetricBalls<1>(N)
    && testExactMetricBalls<4>(N)
    && testExactMetricBalls<8>(N)
    && testInexactMetricBalls(1.4,N)
    && testInexactMetricBalls(43.1,N)
    && testInexactMetricBalls(0.8,N)
    && testChamfer(N)
    && testChamfer2(N)
    && testChamfer3(N)
    ; // && ... other tests
  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
