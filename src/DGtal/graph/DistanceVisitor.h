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

#pragma once

/**
 * @file DistanceVisitor.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5807), University of Savoie, France
 *
 * @date 2012/11/2
 *
 * Header file for template class DistanceVisitor
 *
 * This file is part of the DGtal library.
 */

#if defined(DistanceVisitor_RECURSES)
#error Recursive header files inclusion detected in DistanceVisitor.h
#else // defined(DistanceVisitor_RECURSES)
/** Prevents recursive inclusion of headers. */
#define DistanceVisitor_RECURSES

#if !defined DistanceVisitor_h
/** Prevents repeated inclusion of headers. */
#define DistanceVisitor_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <queue>
#include "DGtal/base/Common.h"
#include "DGtal/base/CountedPtr.h"
#include "DGtal/graph/CUndirectedSimpleLocalGraph.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class DistanceVisitor
  /**
  Description of template class 'DistanceVisitor' <p> \brief Aim: This
  class is useful to perform an exploration of a graph given a
  starting point or set (called initial core) and a distance
  criterion.
 
  The visitor implements a mix of breadth-first algorithm on the
  graph of adjacencies based on a priority queue whose priority is
  given by the distance object. It can be used not only to detect
  connected component but also to identify the layers of the object
  located at a given distance of a starting set.
 
  The \b core of the visitor is by definition at the beginning the set
  of points at the lowest distances. A layer is a set of vertices at
  the same distance. The visitor can visit one vertex at a time or one
  layer at a time. Each layer is at a different distance from the
  initial core, layers having increasing distances.

  The object guarantees that vertices are visited in an non-decreasing
  ordering with respect to the distance object, as long as the
  breadth-first traversal order can be consistent with the given
  distance ordering.

  @tparam TGraph the type of the graph, a model of
  CUndirectedSimpleLocalGraph. It must have an inner type Vertex.

  @tparam TVertexFunctor the type of distance object: any mapping from
  a Vertex toward a scalar value. The neighboring relations of the
  graph should be consistent with the distance function, in the sense
  that the closest points to the current set of already visited
  vertices should be found in the neighbors.

  @tparam TMarkSet the type that is used to store marked
  vertices. Should be a set of Vertex, hence a model of CSet.
 
  @code
     #include "DGtal/geometry/volumes/distance/ExactPredicateLpSeparableMetric.h"
...
     Graph g( ... );
     Graph::Vertex p( ... );
     typedef CanonicSCellEmbedder<KSpace> VertexEmbedder;
     typedef VertexEmbedder::Value RealPoint;
     typedef RealPoint::Coordinate Scalar;
     typedef ExactPredicateLpSeparableMetric<Space,2> ED; // Euclidean distance
     typedef std::binder1st< Distance > EDToPoint;        // Fix one point
     typedef Composer<VertexEmbedder, EDToPoint, Scalar> VertexFunctor; 
       // Compose the vertex embedding with the distance computation.
     typedef DistanceVisitor< Graph, VertexFunctor > Visitor;

     VertexEmbedder embedder;
     ED distance;
     EDToPoint distanceToPoint = std::bind1st( distance, embedder( bel ) );
     VertexFunctor vfunctor( embedder, distanceToPoint );
     DistanceVisitor< Graph, VertexFunctor > visitor( g, p, vfunctor );
     while ( ! visitor.finished() )
       {
         DistanceVisitor<Graph>::Node node = visitor.current();
         std::cout << "Vertex " << node.first 
                   << " at distance " << node.second << std::endl;
         visitor.expand();
       }
   @endcode

   You may also visit vertices layer by layer, meaning that you wish
   to consider all the vertices at the same distance at the same
   time. You should use then DistanceVisitor::getCurrentLayer,
   DistanceVisitor::expandLayer, and DistanceVisitor::ignoreLayer.

@code
std::list<Node> layer;
std::vector<Node> lateLayer;
while ( ! visitor.finished() )
  {
    // Get all vertices at same distance from starting vertex.
    visitor.getCurrentLayer( layer );
    // Do something with the layer ...
    for ( typename std::vector<Node>::const_iterator it = layer.begin(), 
            itE = layer.end(); it != itE; ++it )
      { //... 
      }
    visitor.expandLayer();
  }
@endcode
 
   If the bread-first queueing system is not compatible with the
   distance function, then it has two consequences:

   - in the common distance traversal one at a time, then it may
     happen than a new vertex has a distance less than the preceding
     one.

   - this affects more the layer-by-layer mechanism, which may miss
     vertices in the traversal. In order to visit all the vertices
     layer by layer, you should use the visitor as follows:

     @code
  std::list<Node> layer;
  std::vector<Node> lateLayer;
  while ( ! visitor.finished() )
    {
      // Get all vertices at same distance from starting vertex.
      visitor.getCurrentLayer( layer );
      // Do something with the layer ...
      for ( typename std::vector<Node>::const_iterator it = layer.begin(), 
              itE = layer.end(); it != itE; ++it )
        { //... 
        }
      // Visit the nodes one at a time to expand or ignore depending
      // on your application. Be careful, the nodes may not be in the
      // same order and some other nodes could pop out in-between.
      while ( ! layer.empty() )
        {
          Node n = visitor.current();
          std::list<Node>::iterator it = layer.begin();
          std::list<Node>::iterator itE = layer.end();
          while ( ( it != itE ) && ( (it).first != n.first ) ) ++it;
          if ( it == itE )
            { // Node that is not in the expected order.
              lateLayer.push_back( n );
              myVisitor.ignore();
            }
          else
            { // Process node n, decide to expand or ignore it.
              if ( aBool ) visitor.expand();
              else visitor.ignore();
              // Erase it.
              layer.erase( it );
            }
        }
      // Push back "late" vertices in the visitor.
      for ( typename std::vector<Node>::const_iterator 
              it = lateLayer.begin(), 
              itE = lateLayer.end(); it != itE; ++it )
        {
          visitor.pushAgain( *it );
        }
      lateLayer.clear();
    }
   @endcode

   @see testDistanceVisitor.cpp
   @see testObject.cpp
   */
  template < typename TGraph, 
             typename TVertexFunctor,
             typename TMarkSet = typename TGraph::VertexSet >
  class DistanceVisitor
  {
    // ----------------------- Associated types ------------------------------
  public:
    typedef DistanceVisitor<TGraph,TVertexFunctor,TMarkSet> Self;
    typedef TGraph Graph;
    typedef TVertexFunctor VertexFunctor;
    typedef TMarkSet MarkSet;
    typedef typename Graph::Size Size;
    typedef typename Graph::Vertex Vertex;
    typedef typename VertexFunctor::Value Scalar;
    typedef Scalar Data;

    // Cannot check this since some types using it are incomplete.
    // BOOST_CONCEPT_ASSERT(( CUndirectedSimpleLocalGraph< Graph > ));
    // BOOST_CONCEPT_ASSERT(( CSet< MarkSet, Vertex > ));

    // ----------------------- defined types ------------------------------
  public:

    /**
       The type storing the vertex and its distance. It is also a
       model of boost::LessComparable, boost::EqualityComparable.
    */
    struct Node : public std::pair< Vertex, Scalar > 
    {
      typedef std::pair< Vertex, Scalar > Base;
      using Base::first;
      using Base::second;

      inline Node()
        : std::pair< Vertex, Scalar >()
      {}
      inline Node( const Node & other )
        : std::pair< Vertex, Scalar >( other )
      {}
      inline Node( const Vertex & v, Scalar d )
        : std::pair< Vertex, Scalar >( v, d )
      {}
      inline bool operator<( const Node & other ) const
      {
        return other.second < second;
      }
      inline bool operator<=( const Node & other ) const
      {
        return other.second <= second;
      }
      inline bool operator==( const Node & other ) const
      {
        return other.second == second;
      }
      inline bool operator!=( const Node & other ) const
      {
        return other.second != second;
      }
    };

    /// Internal data structure for computing the distance ordering expansion.
    typedef std::priority_queue< Node > NodeQueue;
    /// Internal data structure for storing vertices.
    typedef std::vector< Vertex > VertexList;

    // /**
    //    Allows to access the node as the pair (Vertex,distance) when
    //    iterating over the graph.
    // */
    // struct NodeAccessor {
    //   typedef const Node value;
    //   typedef const Node value_type;
    //   typedef const Node* pointer;
    //   typedef const Node& reference;
    //   inline
    //   static reference get( const Node & node )
    //   { return node; }
    // };

    // /**
    //    Allows to access the node as only the Vertex when iterating
    //    over the graph.
    // */
    // struct VertexAccessor {
    //   typedef const Vertex value;
    //   typedef const Vertex value_type;
    //   typedef const Vertex* pointer;
    //   typedef const Vertex& reference;
    //   inline
    //   static reference get( const Node & node )
    //   { return node.first; }
    // };

    // /**
    //    This is the base class for constructing an iterator that has
    //    the same behaviour as the traversal made by a
    //    DistanceVisitor. More precisely, the iterator visits in the
    //    same order as a DistanceTraversal object expands without
    //    restriction (no ignore()).

    //    @tparam TAccessor the type that specifies how to access the
    //    visited nodes. Choose VertexAccessor if you only need the
    //    vertex, choose NodeAccessor if you need the pair <Vertex,
    //    Distance>.
    // */
    // template <typename TAccessor>
    // struct ConstIterator 
    // {
    //   typedef ConstIterator<TAccessor> Self;
    //   typedef DistanceVisitor<TGraph,TVertexFunctor,TMarkSet> Visitor;
    //   typedef TAccessor Accessor;

    //   // stl iterator types.
    //   typedef std::input_iterator_tag iterator_category;
    //   typedef typename Accessor::value value_type;
    //   typedef std::ptrdiff_t difference_type; 
    //   typedef typename Accessor::pointer pointer;
    //   typedef typename Accessor::reference reference;

    //   /// Smart pointer to a Visitor.
    //   CountedPtr< Visitor > myVisitor;

    //   inline
    //   ConstIterator() 
    //     : myVisitor( 0 ) {}
    //   inline
    //   ConstIterator( Visitor* ptrV ) 
    //     : myVisitor( ptrV ) {}
    //   inline
    //   ConstIterator( const Self & other ) 
    //     : myVisitor( other.myVisitor ) {}

    //   inline
    //   Self & operator=( const Self & other )
    //   {
    //     if ( this != &other )
    //       myVisitor = other.myVisitor;
    //     return *this;
    //   }

    //   inline
    //   reference
    //   operator*() const
    //   {
    //     ASSERT( ( myVisitor.get() != 0 )
    //             && "DGtal::DistanceVisitor<TGraph,TVertexFunctor,TMarkSet>::ConstIterator::operator*(): you cannot dereferenced a null visitor (i.e. end()).");
    //     return Accessor::get( myVisitor->current() );
    //   }

    //   inline
    //   pointer
    //   operator->() const
    //   { 
    //     ASSERT( ( myVisitor.get() != 0 )
    //             && "DGtal::DistanceVisitor<TGraph,TVertexFunctor,TMarkSet>::ConstIterator::operator->(): you cannot dereferenced a null visitor (i.e. end()).");
    //     return & Accessor::get( operator*() );
    //   }

    //   inline
    //   Self&
    //   operator++()
    //   {
    //     myVisitor->expand();
    //     return *this;
    //   }

    //   inline
    //   Self
    //   operator++(int)
    //   {
    //     Self __tmp = *this;
    //     myVisitor->expand();
    //     return __tmp;
    //   }
      
    //   inline
    //   bool operator==( const Self & other ) const
    //   {
    //     if ( ( myVisitor.get() == 0 ) || myVisitor->finished() )
    //       return ( other.myVisitor.get() == 0 ) || other.myVisitor->finished();
    //     else if ( other.myVisitor.get() == 0 )
    //       return false;
    //     else
    //       return &(myVisitor->current()) == &(other.myVisitor->current());
    //   }

    //   inline
    //   bool operator!=( const Self & other ) const
    //   {
    //     return ! ( this->operator==( other ) );
    //   }
    // };

    // /// const iterator on Vertex for visiting a graph by following a
    // /// distance ordering traversal.
    // typedef ConstIterator<VertexAccessor> VertexConstIterator;
    // /// const iterator on pair (Vertex,distance) for visiting a graph by
    // /// following a distance ordering traversal.
    // typedef ConstIterator<NodeAccessor> NodeConstIterator;

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor.
     */
    ~DistanceVisitor();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    DistanceVisitor ( const DistanceVisitor & other );


    /**
     * Constructor from a point and a vertex functor object. This
     * point provides the initial core of the visitor.
     *
     * @param graph the graph in which the distance ordering traversal takes place (aliased).
     * @param distance the distance object, a functor Vertex -> Scalar (cloned).
     * @param p any vertex of the graph.
     */
    DistanceVisitor( const Graph & graph, 
                     const VertexFunctor & distance,
                     const Vertex & p );

    /**
       Constructor from a graph, a vertex functor and two iterators
       specifying a range. All vertices visited between the iterators
       should be distinct two by two. The so specified set of vertices
       provides the initial core of the distance ordering
       traversal. 
       
       @tparam VertexIterator any type of single pass iterator on vertices.
       @param graph the graph in which the distance ordering traversal takes place (aliased).
       @param distance the distance object, a functor Vertex -> Scalar (cloned).
       @param b the begin iterator in a container of vertices. 
       @param e the end iterator in a container of vertices. 
    */
    template <typename VertexIterator>
    DistanceVisitor( const Graph & graph, 
                     const VertexFunctor & distance,
                     VertexIterator b, VertexIterator e );


    /**
       @return a const reference on the graph that is traversed.
    */
    const Graph & graph() const;

    // ----------------------- traversal services ------------------------------
  public:

    /**
       @return a const reference on the current visited vertex. The
       node is a pair <Vertex,Scalar> where the second term is the
       distance to the initial vertex or set.

       NB: valid only if not 'finished()'.
     */
    const Node & current() const; 

    /**
       @return a const reference on the current visited vertex. The
       node is a pair <Vertex,Scalar> where the second term is the
       distance to the initial vertex or set.

       @tparam TBackInsertable a container of Node that is any model
       of boost::BackInsertable which has also a clear() method.

       NB: Complexity is in O(k log n ), where k is the size of the
       layer and n the number of elements currently in the priority
       queue (some O(k)).
     */
    template <typename TBackInsertable>
    void getCurrentLayer( TBackInsertable & layer );

    /**
       Goes to the next vertex but ignores the current vertex for
       determining the future visited vertices. Otherwise said, no
       future visited vertex will have this vertex as a father.

       NB: valid only if not 'finished()'.
     */
    void ignore();

    /**
       Goes to the next layer but ignores the current layer for
       determining the future visited vertices. Otherwise said, no
       future visited vertex will have any vertex of this layer as a
       father.

       NB: valid only if not 'finished()'.
     */
    void ignoreLayer();

    /**
       Goes to the next vertex and take into account the current
       vertex for determining the future vsited vertices.
       NB: valid only if not 'finished()'.
     */
    void expand();

    /**
       Goes to the next layer and take into account the current
       layer for determining the future vsited vertices.
       NB: valid only if not 'finished()'.
     */
    void expandLayer();

    /**
       Goes to the next vertex and take into account the current
       vertex for determining the future visited vertices.

       @tparam VertexPredicate a type that satisfies CPredicate on Vertex.

       @param authorized_vtx the predicate that should satisfy the
       visited vertices.

       NB: valid only if not 'finished()'.
     */
    template <typename VertexPredicate>
    void expand( const VertexPredicate & authorized_vtx );

    /**
       Goes to the next layer and take into account the current
       layer for determining the future vsited vertices.

       @tparam VertexPredicate a type that satisfies CPredicate on Vertex.

       @param authorized_vtx the predicate that should satisfy the
       visited vertices.

       NB: valid only if not 'finished()'.
     */
    template <typename VertexPredicate>
    void expandLayer( const VertexPredicate & authorized_vtx );
    
    /**
       @return 'true' if all possible elements have been visited.
     */
    bool finished() const;

    /**
       Force termination of the distance ordering traversal. 'finished()'
       returns 'true' afterwards and 'current()', 'expand()',
       'ignore()' have no more meaning. Furthermore,
       'markedVertices()' and 'visitedVertices()' both represents the
       set of visited vertices.
     */
    void terminate();

    /**
       @return a const reference to the current set of marked
       vertices. It includes the visited vertices and the vertices
       neighbors to the current layer of vertices. NB: O(1) operation.
     */
    const MarkSet & markedVertices() const;

    /**
       @return the current set of visited vertices (a subset of marked
       vertices; excludes the marked vertices yet to be visited).
       Note that if 'finished()' is true, then 'markedVertices()' is
       equal to 'visitedVertices()' and should thus be preferred. 

       NB: computational cost is a copy of the set of marked vertices
       then as many deletion as the number of marked vertices yet to
       be visited.

       @see markedVertices
     */
    MarkSet visitedVertices() const;

    /**
       Push backs some node in the queue. The node should have been
       ignored previously at some point. Useful when the distance is
       not truely a distance function.
       @param node a pair Vertex, distance.
     */
    void pushAgain( const Node & node );


    /**
       Exchange 'this' object with 'other'. O(1) operations since
       containers are swapped (if the VertexFunctor is assignable in O(1)).
       @param other the other instance.
    */
    void swap( DistanceVisitor & other );

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  private:
    // ------------------------- Private Datas --------------------------------
  private:

    /**
     * The graph where the traversal takes place.
     */
    const Graph* myGraph;

    /**
       The distance object, a functor Vertex -> Scalar.
    */
    VertexFunctor myDistance;

    /**
     * Set representing the marked vertices: the ones that have been
     * visited and the ones that are going to be visited soon (at
     * closest distance).
     */
    MarkSet myMarkedVertices;

    /**
       Queue storing the vertices that are the next visited ones in
       the distance ordering traversal of the graph.
     */
    NodeQueue myQueue;

    // ------------------------- Hidden services ------------------------------
  protected:

    /**
     * Constructor.
     * Forbidden by default (protected to avoid g++ warnings).
     */
    DistanceVisitor();

  private:

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     * Forbidden by default.
     */
    DistanceVisitor & operator= ( const DistanceVisitor & other );

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class DistanceVisitor


  /**
   * Overloads 'operator<<' for displaying objects of class 'DistanceVisitor'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'DistanceVisitor' to write.
   * @return the output stream after the writing.
   */
  template < typename TGraph, typename TVertexFunctor, typename TMarkSet >
  std::ostream&
  operator<< ( std::ostream & out, 
               const DistanceVisitor<TGraph,TVertexFunctor,TMarkSet> & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/topology/DistanceVisitor.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DistanceVisitor_h

#undef DistanceVisitor_RECURSES
#endif // else defined(DistanceVisitor_RECURSES)
