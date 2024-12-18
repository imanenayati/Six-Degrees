/**
 ** CollabGraph.h
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
**    2) The graph is undirected
 **   3) Loops (edges from a vertex to itself) are forbidden
 **   4) Non-existent edges are represented by the empty string
 **   5) Artists with the empty string as their name are forbidden 
 **
 ** ChangeLog:
 **   17 Nov 2020: zgolds01
 **   April 2021: CR Calabrese
 **
 **/

#ifndef __COLLAB_GRAPH__
#define __COLLAB_GRAPH__

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Artist.h"

class CollabGraph {

public:

        /******************************************************
         * NOTE: PLEASE DO NOT CHANGE THE SIGNATURES          *
         * OF ANY FUNCTIONS IN THE COLLABGRAPH.               *
         * You may add private helper functions if you wish.  *
         * ****************************************************/

    /* Nullary Constructor */
    CollabGraph();

    /* The 'Big Three' */
    ~CollabGraph();
    CollabGraph(const CollabGraph &source);
    CollabGraph &operator=(const CollabGraph &rhs);

    /* Mutators */
    void insert_vertex(const Artist &artist);
    void insert_edge(const Artist &a1, const Artist &a2,
                     const std::string &song);
    void mark_vertex(const Artist &artist);
    void unmark_vertex(const Artist &artist);
    void set_predecessor(const Artist &to, const Artist &from);
    void clear_metadata();    

    /* Accessors */
    bool                is_vertex(const Artist &artist) const;
    bool                is_marked(const Artist &artist) const;
    Artist              get_predecessor(const Artist &artist) const;
    std::string         get_edge(const Artist &a1, const Artist &a2) const;

    /* student TODO: get_vertex_neighbors and report_path */
    std::vector<Artist> get_vertex_neighbors(const Artist &artist) const;
    std::stack<Artist>  report_path(const Artist &source,
                                    const Artist &dest) const;


    void                print_graph(std::ostream &out);

private:
    struct Edge {
        Edge(Artist a, std::string s) {
            neighbor = a; 
            song = s; 
        };
        Artist neighbor;
        std::string song;
    };

    struct Vertex {
        Vertex() {};
        Vertex(Artist a) { artist = a; };

        Artist artist;
        std::vector<Edge> neighbors;

        Vertex *predecessor = nullptr;
        bool visited = false;
    };

    void self_destruct();
    void enforce_valid_vertex(const Artist &artist) const;

    std::unordered_map<std::string, std::unique_ptr<Vertex>> graph;
};

#endif /* __COLLAB_GRAPH__ */