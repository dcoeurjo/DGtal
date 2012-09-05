#include "DGtal/io/readers/VolReader.h"
#include "DGtal/io/readers/LongvolReader.h"
#include "DGtal/io/writers/VolWriter.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/helpers/StdDefs.h"
#include <Qt/qapplication.h>
#include "DGtal/io/viewers/Viewer3D.h"
#include "DGtal/base/Common.h"

#include "DGtal/geometry/tools/Watershed.h"

#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/colormaps/GrayscaleColorMap.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <math.h>

using namespace DGtal;
using namespace Z3i;
namespace po = boost::program_options;

typedef ImageContainerBySTLVector < Z3i::Domain, int> Image;

template <typename TValue> class comp
{
public:
  comp() {}
  bool
  operator() (const TValue& i1, const TValue& i2)
  {
    return i1 > i2;
  }
};



int main(int argc, char** argv)
{
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("Input,i", po::value<std::string>(), "input longvol file name")
    ("Output,o", po::value<std::string>(), "output vol file name")
    ("Default,d", "compute a standard watershed")
    ("StochasticProbability,p", "compute the stochastic probability function")
    ("StochasticWatershed,w", "compute a watershed based on a stochastic probability function")
    ("Iterations,M", po::value<int>(), "number of times a watershed is executed to calclate the stochastic probability function")
    ("Regions,N", po::value<int>(), "estimated number of regions (stochastic only)");
  
  
  bool parseOK=true;
  po::variables_map vm;
  try
    {
      po::store(po::parse_command_line(argc, argv, general_opt), vm);  
    } 
  catch(const std::exception& ex)
    {
      parseOK=false;
      trace.info()<< "Error checking program options: "<< ex.what()<< endl;
    }
  
  po::notify(vm);    
  
  if(!parseOK || vm.count("help") || argc<=1 || (!(vm.count("Input"))) || (!(vm.count("Output"))) )
    {
      trace.info()<< "Watershed" <<std::endl << "Basic usage: "<<std::endl
		  << "\t Watershed [options] --Input  <fileName> --Output <fileName> --Default"<<std::endl
		  << general_opt << "\n";
      return 0;
    }
  if( vm.count("Default") + vm.count("StochasticProbability") + vm.count("StochasticWatershed") != 1 )
    {
      trace.info()<< "Watershed" <<std::endl 
		  << "Only one watershed method argument may be used "
		  << "(--Default, --StochasticProbability or --StochasticWatershed)" << std::endl;
      return 0;
    }
  if( ( vm.count("StochasticProbability") || 
	vm.count("StochasticWatershed") ) && 
      ( !vm.count("Iterations") || !vm.count("Regions") ) )
    {
      trace.info()<< "Watershed" <<std::endl 
		  << "Missing options : --Iterations <integer> --Regions <integer>" << std::endl;
      return 0;
    }


  // Initialisation
  QApplication application(argc,argv);
  srand ( time(NULL) );
    
  // Import longvol image
  trace.beginBlock("Loading image");
  std::string LVinputFilename = vm["Input"].as<string>();
  std::string outputFilename = vm["Output"].as<string>();
   
  Image LVimage = LongvolReader<Image>::importLongvol(LVinputFilename);
  
  Z3i::DigitalSet LVset3d (LVimage.domain());
  SetFromImage<Z3i::DigitalSet>::append<Image>(LVset3d, LVimage, 0,10000);
  
  trace.endBlock();
    
  // ColorMap definition
  GradientColorMap<int> cmap_grad( 0, 255 );
    cmap_grad.addColor( Color( 255, 0, 0 ) );
    cmap_grad.addColor( Color( 255, 255, 0 ) );
    cmap_grad.addColor( Color( 0, 255, 0 ) );
    cmap_grad.addColor( Color( 0, 255, 255 ) );
    cmap_grad.addColor( Color( 0, 0, 255 ) );
    cmap_grad.addColor( Color( 255, 0, 255 ) );
  
  typedef Object18_6 Object;
  
  Object object = Object(Z3i::dt18_6, LVset3d);
    
  // Watershed launching
  Watershed<Object, Image, std::greater<Image::Value> > ws(object, LVimage);
  
  
  Image WSImage = LVimage;
  
  if( vm.count("StochasticProbability") || vm.count("StochasticWatershed") )
    {
      int N = vm["Regions"].as<int>();
      int M = vm["Iterations"].as<int>();
      if( vm.count("StochasticProbability") )
	{
	  WSImage = ws.segmentationStochastic(N, M);
	}
      else
	{
	  WSImage = ws.segmentationStochasticHeuristic(N, M);
	}
    }
  else
    {
      WSImage = ws.segmentation();
    }
  
  Domain WSdomain = WSImage.domain();
  
  
  
  VolWriter<Image, GrayscaleColorMap<int> >::exportVol(outputFilename, WSImage,0, 255);
  
  // Display
  Viewer3D viewer;
  viewer.show();  
  //viewer << LVset3d ;
  
  for(Domain::ConstIterator it = WSdomain.begin(), itend=WSdomain.end(); it!=itend; ++it){
    int value = WSImage( *it );
    if( value == -2 ) continue;
    if( value != 0 )
      {
	if( value == 255 )
	  {
	    viewer << CustomColors3D(Color(0, 0, 0), Color(255,255,255));
    }
	else
	  {
	    if (value > 0) 
	      viewer << CustomColors3D(Color(0, 0, 0), Color(cmap_grad(value).red(), cmap_grad(value).green(), cmap_grad(value).blue(), 255) );
	    else
	      viewer << CustomColors3D(Color(0,0,0), Color(0, 100, 0));
	  }
	viewer << *it;
      }
  }
  
  
  
  viewer << Viewer3D::updateDisplay;
  
  return application.exec();
  return 0;
}
