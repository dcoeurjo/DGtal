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
 * @file testWatershed.cpp
 * @ingroup Tests
 * @author Jérémy Gaillard (\c jeremy.gaillard@insa.lyon.fr )
 * Institut National des Sciences Appliquées - INSA, France
 *
 *
 * @date 2012/07/05
 *
 * This file is part of the DGtal library
 */

/**
 * @brief Aim: simple test of Watershed
 */

#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "DGtal/base/Common.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/io/readers/PNMReader.h"
#include "DGtal/geometry/tools/Watershed.h"
#include "DGtal/io/writers/PGMWriter.h"

#include "ConfigTest.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;


struct IntToUnsignedChar
{
  unsigned char operator()(const int a) const
  {
    return static_cast<unsigned char>(a);
  }
};

typedef ImageContainerBySTLVector < Domain, int > Image;

int main(int argc, char **argv)
{
  //! [Initialization]
  std::string filename( argv[1] );
  Image image = PNMReader<Image>::importPGM( filename ); 

  trace.info() << image<<std::endl;

  DigitalSet set(image.domain());
  
  SetFromImage<DigitalSet>::append<Image>(set, image, 0, 255);
  trace.info() << "Vertex set="<<set<<std::endl;
  Object4_8 object(dt4_8, set);
  //! [Initialization]
  
  trace.beginBlock("Simple Watershed test");
  
  //! [WatershedLaunch]
  typedef Watershed<Object4_8, Image> watershed;
  watershed ws(object, image);
  Image result = ws.segmentation();
  //! [WatershedLaunch]
  
  trace.endBlock();
  
  Board2D board;

  board << SetMode( image.domain().className(), "Paving" )
	<< image.domain()
	<< SetMode( image.domain().lowerBound().className(), "Paving" );
  string specificStyle = image.domain().lowerBound().className() + "/Paving";
  
  int maxval=0;
  for(Image::ConstRange::ConstIterator it = result.constRange().begin(), 
	itend = result.constRange().end(); it != itend; ++it)
    if ((*it) != ws.getWatershedValue()  && ( (*it > maxval)))
      maxval = *it;

  trace.info() << "Max val = "<<maxval<<std::endl;
  GradientColorMap<int> cmap_grad( 1, maxval );
  cmap_grad.addColor( Color( 50, 50, 255 ) );
  cmap_grad.addColor( Color( 255, 0, 0 ) );
  cmap_grad.addColor( Color( 255, 255, 10 ) );
  
  for ( typename Object4_8::DigitalSet::ConstIterator it = object.pointSet().begin();
  it != object.pointSet().end();
  ++it )
  {
    if( result(*it) == ws.getWatershedValue() )
    {
      board << CustomStyle( specificStyle,
	  new CustomColors( Color::Black,
	  Color::Black ) )
	<< *it;
    }
    else
    {
      board << CustomStyle( specificStyle,
	  new CustomColors( Color::Black,
	  cmap_grad( result(*it) ) ) )
	<< *it;
    }
    
  }
  
  PGMWriter<Image, IntToUnsignedChar>::exportPGM("testWatershedFromImage.pgm",result , IntToUnsignedChar()  );

  return 0;
}
