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
 * @file
 * @ingroup Tests
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systemes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2020/03/24
 *
 * Functions for testing class MeshTextureHelpers.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "DGtal/base/Common.h"
#include "ConfigTest.h"
#include "DGtalCatch.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/shapes/TriangulatedSurface.h"
#include "DGtal/shapes/MeshTextureHelpers.h"

#ifdef WITH_VISU3D_QGLVIEWER
#include "DGtal/io/viewers/Viewer3D.h"
#endif
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class MeshTextureHelpers.
///////////////////////////////////////////////////////////////////////////////

TEST_CASE( "Testing MeshTextureHelpers" )
{
  typedef PointVector<3,double>             RealPoint;
  typedef TriangulatedSurface< RealPoint >  TriMesh;
  TriMesh mesh;
  mesh.addVertex( RealPoint( 1, 0, 0 ) );
  mesh.addVertex( RealPoint( 0, 1, 0 ) );
  mesh.addVertex( RealPoint( 0, 0, 1 ) );
  mesh.addTriangle( 0, 1, 2 );
  mesh.build();
  
  SECTION("Testing Barycentric Coordinates")
  {
    //middle not in the plane
    RealPoint p(0.5,0.5,0.5);
    RealPoint lambda(0.333333,0.3333333,0.333333);
    RealPoint bary = MeshTextureHelpers<RealPoint>::getBarycentricCoordinatesInFace(mesh,0, p);
    INFO("Bary p"<<bary);
    REQUIRE(bary[0] == Approx(lambda[0]));
    REQUIRE(bary[1] == Approx(lambda[1]));
    REQUIRE(bary[2] == Approx(lambda[2]));
    
    //vertex
    RealPoint p2(1,0,0);
    bary = MeshTextureHelpers<RealPoint>::getBarycentricCoordinatesInFace(mesh,0, p2);
    RealPoint reconstruction = MeshTextureHelpers<RealPoint>::getPointFromBarycentricCoordinatesInFace(mesh,0, bary);
    INFO("Bary p2 "<<bary);
    INFO("p2 reco "<<reconstruction);
    REQUIRE(p2[0] == Approx(reconstruction[0]));
    REQUIRE(p2[1] == Approx(reconstruction[1]));
    REQUIRE(p2[2] == Approx(reconstruction[2]));
    
    //on edge
    RealPoint p3(.5,.5,0);
    bary = MeshTextureHelpers<RealPoint>::getBarycentricCoordinatesInFace(mesh,0, p3);
    reconstruction = MeshTextureHelpers<RealPoint>::getPointFromBarycentricCoordinatesInFace(mesh,0, bary);
    INFO("Bary "<<bary);
    INFO("p3 reco "<<reconstruction);
    REQUIRE(p3[0] == Approx(reconstruction[0]));
    REQUIRE(p3[1] == Approx(reconstruction[1]));
    REQUIRE(p3[2] == Approx(reconstruction[2]));
  }
  
  SECTION("Test OBJ loader")
  {
    TriMesh cube;
    MeshTextureHelpers<RealPoint>::UVMap texture;
    MeshTextureHelpers<RealPoint>::UVMesh uvMesh;
    MeshTextureHelpers<RealPoint>::NormalMesh normalMesh;
    MeshTextureHelpers<RealPoint>::NormalMap normalMap;
    std::tie( cube , uvMesh, normalMesh, texture, normalMap) = MeshTextureHelpers<RealPoint>::loadOBJWithTextureCoord(testPath + "samples/cubetext.obj");
    REQUIRE(cube.nbVertices() == 8);
    REQUIRE(cube.nbFaces() == 12);
    
    //Dumping OBJ
    std::ofstream out;
    out.open ("dump.obj");
    for(auto v=0; v < cube.nbVertices(); ++v)
    {
      auto p=cube.position(v);
      out<<"v "<<p[0]<<" "<<p[1]<<" "<<p[2]<<std::endl;
    }
    out<<std::endl;
    for(auto &v: texture)
      out<<"vt "<<v[0]<<" "<<v[1]<<std::endl;
    out<<std::endl;
    for(auto &v: normalMap)
      out<<"vn "<<v[0]<<" "<<v[1]<<" "<<v[2]<<std::endl;
    out<<std::endl;
    
    for(auto f=0; f < cube.nbFaces(); ++f)
    {
      auto vert = cube.verticesAroundFace(f);
      auto uvtriangle = uvMesh[f];
      auto ntriangle = normalMesh[f];
      out<< "f "<< vert[0]+1<<"/"<<uvtriangle[0]+1<<"/"<<ntriangle[0]+1<<" "
                                   << vert[1]+1<<"/"<< uvtriangle[1]+1<<"/"<<ntriangle[0]+1<< " "
                                   << vert[2]+1<<"/"<< uvtriangle[2]+1<<"/"<<ntriangle[0]+1<<std::endl;
    }
    out.close();
  }
  
  SECTION("Test Texture features")
  {
    auto image = MeshTextureHelpers<RealPoint>::loadTexture(testPath + "samples/UVchecker.png");
    INFO(image);
    REQUIRE(image.isValid());
    MeshTextureHelpers<RealPoint>::UV center(0.5,0.5);
    INFO(MeshTextureHelpers<RealPoint>::textureFetch(image, center));
    REQUIRE( MeshTextureHelpers<RealPoint>::textureFetch(image, center) == Color(24,19,0,255));
  }
}

/** @ingroup Tests **/