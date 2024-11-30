/**
 ** CollabGraph.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Represent a collaboration graph of artists, where each artist is
 **   a vertex and an edge between two artists is a song in which those
 **   artists have collaborated. Accessor and mutator functions are provided,
 **   allowing for convenient traversal of the graph.
 **
 ** Notes:
 **   1) Implemented with adjacency lists (not an adjacency matrix)
 **   2) The graph is undirected
 **   3) Non-existent edges are represented by the empty string
 **   4) Artists with the empty string as their name are forbidden 
 **   5) Loops (edges from a vertex to itself) are forbidden
 **
 ** ChangeLog:
 **   17 Nov 2020: zgolds01
 **       CollabGraph class created
 **   April 2021: CR Calabrese
 **       Edited the CollabGraph to use an Edge struct & remove vector of
 **       pointers to all vertices.
 **       Edited function contracts to improve consistency with course style
 **       guide and other starter code.
 **
 **/

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <functional>
#include <exception>
#include <sstream>
#include <fstream>
#include <memory>

#include "Artist.h"
#include "CollabGraph.h"

using namespace std;

/*********************************************************************
 ******************** public function definitions ********************
 *********************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: nullary constructor
 * @purpose: initialize a CollabGraph instance
 *
 * @preconditions: none
 * @postconditions: none
 *
 * @parameters: none
 */
CollabGraph::CollabGraph()
{
    
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: destructor
 * @purpose: deallocate all memory associated with this CollabGraph
 *           instance
 *
 * @preconditions: none
 * @postconditions: all heap-allocated memory associated with this CollabGraph
 *                  instace is freed
 */
CollabGraph::~CollabGraph() {
    graph.clear();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: copy constructor
 * @purpose: deeply copy another CollabGraph instance
 *
 * @preconditions: none
 * @postconditions: this instance is a deep copy of the provided
 *                  CollabGraph reference
 *
 * @parameters: a const CollabGraph reference, to be deeply copied
 */
CollabGraph::CollabGraph(const CollabGraph &source)
{
    *this = source;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * assignment operator overload
 * purpose: deeply copy another CollabGraph instance
 *
 * parameters: a const CollabGraph reference, to be deeply copied
 * returns: a CollabGraph reference
 */
CollabGraph &CollabGraph::operator=(const CollabGraph &rhs) {
    if (this != &rhs) {
        self_destruct();

        for (auto &pair : rhs.graph) {
            std::unique_ptr<Vertex> vertexCopy = std::make_unique<Vertex>();
            vertexCopy->artist = pair.second->artist;
            vertexCopy->neighbors = pair.second->neighbors;
            vertexCopy->predecessor = nullptr;  
            vertexCopy->visited = false;       

            graph[pair.first] = std::move(vertexCopy);
        }
    }
    return *this;
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * insert_vertex
 * purpose: insert a vertex in the collaboration graph
 *
 * parameters: a const Artist reference to use as a vertex in the graph
 * returns: none
 * 
 * notes: 1) if the artist is already in the graph, then nothing happens
 *        2) throws a runtime_error iff the provided Artist has the empty string
 *           as its name, since an Artist instance with the empty string as its
 *           name is improperly initialized
 */
void CollabGraph::insert_vertex(const Artist &artist) {
    if (artist.get_name().empty()) {
        throw runtime_error(
            "cannot insert an improperly initialized Artist instance ");
        throw runtime_error(
             "(name must be non-empty)");

    }


    if (graph.find(artist.get_name()) == graph.end()) {

        graph[artist.get_name()] = std::make_unique<Vertex>(artist);
    }
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * insert_edge
 * purpose: insert an edge between two vertices in the collaboration graph
 *
 * preconditions: 1) both 'a1' and 'a2' are vertices in the graph
 * postconditions: an edge with name 'edgeName' is inserted between 'a1' and
 *                  'a2' iff there was not already an edge connecting those
 *                  vertices
 *
 * parameters: 1) a const Artist reference, which should be a vertex in 
 *                the graph
 *             2) a const Artist reference, which should be a different vertex
 *                in the graph
 *             3) a const string reference, the name of the edge that will
 *                connect the two provided vertices
 * returns:    none
 * 
 * notes: throws a runtime_error, if:
 *            1) an empty string is provided as the 'edgeName', since this
 *               would violate the representation invariant that empty strings
 *               represent non-existent edges
 *        OR, 2) 'a1' and 'a2' are the same vertex, because creating an edge
 *               between a vertex and itself would product a loop (which would
 *               very likely result in an infinite loop during traversal)
 */
void CollabGraph::insert_edge(const Artist &a1, const Artist &a2,
                              const string &edgeName)
{
    enforce_valid_vertex(a1);
    enforce_valid_vertex(a2);

    if (edgeName == "") {
        string message = "the empty string is not a valid edge name";
        throw runtime_error(message.c_str());
    }

    if (a1 == a2) {
        string message = "cannot insert an edge between a "
                              "vertex and itself";
        throw runtime_error(message.c_str());
    }

    if (get_edge(a1, a2) != "") return;

    graph.at(a1.get_name())->neighbors.push_back(Edge(a2, edgeName));
    graph.at(a2.get_name())->neighbors.push_back(Edge(a1, edgeName));
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * mark_vertex
 * purpose: mark the given vertex as visited
 *
 * parameters: an Artist instance, which should correspond to a vertex 
 *             in the graph
 * returns:    none
 */
void CollabGraph::mark_vertex(const Artist &artist)
{
    enforce_valid_vertex(artist);
    graph.at(artist.get_name())->visited = true;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * unmark_vertex
 * purpose: mark the given vertex as not visited
 *
 * parameters: an Artist instance, which should correspond to a vertex 
 *             in the graph
 * returns:    none
 */
void CollabGraph::unmark_vertex(const Artist &artist)
{
    enforce_valid_vertex(artist);
    graph.at(artist.get_name())->visited = false;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * set_predecessor
 * purpose: update the predecessor field of the 'to' vertex for path 
 *          generation
 *
 * parameters: 1) a const Artist reference, the vertex those predecessor
 *                will be set. should be a vertex in the graph
 *             2) a const Artist reference, the predecessor. should be a
 *                vertex in the graph.
 * 
 * returns:    none
 * note:       does nothing if 'to' already has a predecessor
 *             when the path is generated, the path will move from the
 *             'from' (predecessor) vertex to the 'to' vertex
 */
void CollabGraph::set_predecessor(const Artist &to, const Artist &from) {
    enforce_valid_vertex(to);
    enforce_valid_vertex(from);

    auto& to_vertex = graph.at(to.get_name());
    auto& from_vertex = graph.at(from.get_name());

    if (to_vertex->predecessor == nullptr) {
        to_vertex->predecessor = from_vertex.get();
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * clear_metadata
 * purpose: clears the metadata in the graph, preparing the graph for
 *          another traversal
 *
 * postconditions: 1) all vertices in the graph are marked as unvisited
 *                  2) the predecessor of each vertex is set to 'nullptr'
 *
 * parameters: none
 * returns: none
 * 
 * warning: this function MUST be called before each traversal, and failure
 *           to heed this warning will result in undefined behavior
 */
void CollabGraph::clear_metadata()
{
    /* Again, we use the std::iterator to iterate over an unordered_map. 
     * Whereas the graph.at() function returns just the "value", when we
     * iterate we're getting the (key, value) pairs, so we have to 
     * traverse to the second element in the pair to get the vertex itself. */
    for (auto itr = graph.begin(); itr != graph.end(); itr++) {
        itr->second->visited = false;
        itr->second->predecessor = nullptr;
    }
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * is_vertex
 * purpose: determine whether an artist maps to a vertex in the
 *          collaboration graph
 *
 * parameters: a const Artist referece
 * returns: a bool, true iff the provided artist is in the graph
 */
bool CollabGraph::is_vertex(const Artist &artist) const
{
    return graph.find(artist.get_name()) != graph.end();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * is_marked
 * purpose: determine whether the provided vertex has been visited
 *
 * parameters: a const Artist reference, which should be in the graph
 * returns: a bool, true iff the provided vertex has been visited
 */
bool CollabGraph::is_marked(const Artist &artist) const
{
    enforce_valid_vertex(artist);
    return graph.at(artist.get_name())->visited;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * get_predecessor
 * purpose: retrieve predecessor of a vertex in the collaboration graph
 *
 * parameters: a const Artist reference, which should be a vertex in the graph
 * returns:    an Artist, which is the predecessor of the provided 'artist'
 *             in the collaboration graph; or an artist with the empty string
 *             as the name if the given artist has no predecessor
 */
Artist CollabGraph::get_predecessor(const Artist &artist) const
{
    enforce_valid_vertex(artist);

    Artist pred_artist;
    
    
    Vertex *pred_vertex = graph.at(artist.get_name())->predecessor;

    if (pred_vertex != nullptr)
        pred_artist = pred_vertex->artist;

    return pred_artist;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * get_edge
 * purpose: retrieve the edge between two vertices in the collaboration graph
 *
 * parameters: 1) a const Artist reference, which should be in the graph
 *             2) a const Artist reference, which should be in the graph
 * returns:    a string, which is the name of the edge connecting 'a1' and
 *             'a2', or the empty string if there is no edge connecting them
 */
string CollabGraph::get_edge(const Artist &a1, const Artist &a2) const
{
    enforce_valid_vertex(a1);
    enforce_valid_vertex(a2);

    vector<Edge> adj = graph.at(a1.get_name())->neighbors;

    size_t idx = 0;
    bool found = false;
    while (not found and idx < adj.size()) {
        if (adj[idx].neighbor == a2) {
            found = true;
        }
        else idx++;
    }

    if (idx < adj.size()) return adj[idx].song;
    else                  return "";
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * get_vertex_neighbors
 * purpose: retrieve the neighbors of a vertex in the collaboration graph
 *
 * parameters: a const Artist reference, which should be in the graph
 * returns:    a vector of Artist instances, which contains all neighbors
 *             of the provided vertex
 */
vector<Artist> CollabGraph::get_vertex_neighbors(const Artist &artist) const
{
    vector<Artist> neighbors;

    enforce_valid_vertex(artist);

    const vector<Edge> &edges = graph.at(artist.get_name())->neighbors;

    for (const Edge &edge : edges) {
        neighbors.push_back(edge.neighbor);
    }

    return neighbors;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * report_path
 * purpose: accumualate the path from the provided 'source' vertex to the
 *           provided 'dest' vertex
 *
 * preconditions: 1) the graph has been traversed, and the predecessor field
 *                   of all vertices between 'source' and 'dest' has been
 *                   properly set
 *
 * parameters: 1) a const Artist reference, the source vertex.
 *                should be in the graph
 *             2) a const Artist reference, the destination vertex.
 *                should be in the graph
 * returns:    a stack of Artist instances representing the path from the 
 *             source to the destination, where the top-most element is the
 *             source vertex, and the bottom-most element is the
 *             destination vertex
 */
stack<Artist> CollabGraph::report_path(const Artist& source, const Artist& dest) const {
    stack<Artist> path;

    enforce_valid_vertex(source);
    enforce_valid_vertex(dest);

    if (source == dest) {
        return path;
    }

    Artist current = dest;

    while (current != source && current.get_name() != "") {
        path.push(current);
        current = get_predecessor(current);
    }

    if (current == source) {
        path.push(source); 
    } else {
        return stack<Artist>();
    }

    return path;
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * function: print_graph
 * purpose: print a representation of the information stored in the
 *          collaboration graph
 *
 * parameters: a ostream reference, where output is sent
 * returns: none
 */
void CollabGraph::print_graph(ostream &out)
{
    for (auto itr = graph.begin(); itr != graph.end(); itr++) {

        vector<Edge> neighbors = itr->second->neighbors;

        for (size_t i = 0; i < neighbors.size(); i++) {
            out << "\"" << itr->second->artist.get_name() << "\" "
                << "collaborated with "
                << "\"" << neighbors.at(i).neighbor << "\" in "
                << "\"" << neighbors.at(i).song << "\"."
                << endl;
        }

        out << "***" << endl;
    }

}





/**********************************************************************
 ******************** private function definitions ********************
 **********************************************************************/



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * self_destruct
 * purpose: free all memory associated with this CollabGraph
 *          instance
 *
 * parameters: none
 * returns: none
 */
void CollabGraph::self_destruct() {
    graph.clear(); // Automatically deletes managed objects
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * enforce_valid_vertex
 * purpose: ensure that the given artist is in the graph; throw an error if it
 *          is not. if an exception is not thrown, then the provided Artist
 *          instance maps to a vertex in the collaboration graph
 *
 * parameters: a const Artist reference, which should map to a vertex in
 *             the collaboration graph
 * returns: none
 */
void CollabGraph::enforce_valid_vertex(const Artist &artist) const
{
    if (not is_vertex(artist)) {
        string message = "artist \"" + artist.get_name() + \
                              "\" does not exist in the collaboration graph";
        throw runtime_error(message.c_str());
    }
}