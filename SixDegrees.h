/*
 * SixDegrees.h
 *
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Iman Enayati, April 18th 2024
 */
#ifndef __SIX_DEGREES__
#define __SIX_DEGREES__

#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>

#include "Artist.h"
#include "CollabGraph.h"

using namespace std;

class SixDegrees {
    public:

        SixDegrees();

        void populateGraph(vector<Artist *> artistList);
        void setEdges(vector<Artist *> artistList);

        void print();
        void print_path();

        void clear_metadata();

        void bfs_function(const Artist &from, const Artist &to);
        void dfs_function(const Artist &from, const Artist &to);
        void not_function(const Artist &from, const Artist &to, 
                                                vector<Artist> notList);

        void exclude(const Artist &exclude);
        

        void configureOutput(ostream *out);
        ostream *output;
        
    private:
        CollabGraph graph;
        bool pathFound = false;

        void print_path(const Artist &from, const Artist &to);
        bool validate_from_to(const Artist &from, const Artist &to);
        bool validateList(vector<Artist> List);
        bool validateExclude(const Artist &exclude);
        void dfs_private(const Artist &curArtist, const Artist &parent,
                                       const Artist &from, const Artist &to);
};

#endif /* __SIX_DEGREES__ */