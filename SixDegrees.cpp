/*
 * SixDegrees.cpp
 *
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Iman Enayati, April 18th 2024
 */
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <exception>

#include "SixDegrees.h"
#include "Artist.h"
#include "CollabGraph.h"

using namespace std;

/*
 * @constructor: SixDegrees
 * @purpose: Constructs a SixDegrees object, 
 * initializing the graph and output stream.
 *
 * @preconditions: None.
 *
 * @postconditions: A SixDegrees object is 
 * created with an initialized graph and 
 * output stream.
 *
 * @parameters: None.
 *
 * @returns: None.
 */

SixDegrees::SixDegrees()
{
    graph = CollabGraph();
    output = &cout;
}

/*
 * @function: populateGraph
 * @purpose: Populates the graph with artists 
 * and sets edges based on their collaborations.
 *
 * @preconditions: None.
 *
 * @postconditions: The graph is populated with 
 * artists from 'artistList', and edges are set 
 * based on their collaborations.
 *
 * @parameters: artistList - A list of pointers 
 * to artists (vector<Artist*>).
 *
 * @returns: None.
 */

void SixDegrees::populateGraph(vector<Artist *> artistList)
{
    for (Artist *artistToAdd : artistList) {
        graph.insert_vertex(*artistToAdd);
    }

    setEdges(artistList);
}

/*
 * @function: setEdges
 * @purpose: Sets edges between artists 
 * based on their collaborations.
 *
 * @preconditions: The graph is initialized. 
 * 'artistList' contains valid pointers to artists.
 *
 * @postconditions: Edges are set between artists 
 * based on their collaborations.
 *
 * @parameters: artistList - A list of pointers 
 * to artists (vector<Artist*>).
 *
 * @returns: None.
 */

void SixDegrees::setEdges(vector<Artist *> artistList)
{
    for (auto out = artistList.begin(); out != artistList.end(); out++) {
        for (auto in = artistList.begin(); in != artistList.end(); in++) {
            if (out == in)
            {
                continue;
            }

            string edgeName = "";
            edgeName = (*out)->get_collaboration(**in);

            if(edgeName == "") {
                continue;
            }
            graph.insert_edge(**out, **in, edgeName);
        }
    }
}

/*
 * @function: print
 * @purpose: Prints the graph.
 *
 * @preconditions: The graph is initialized.
 *
 * @postconditions: None.
 *
 * @parameters: None.
 *
 * @returns: None.
 */

void SixDegrees::print()
{
    graph.print_graph(*output);
}

/*
 * @function: clear_metadata
 * @purpose: Clears metadata related to graph 
 * traversal.
 *
 * @preconditions: None.
 *
 * @postconditions: Metadata related to graph 
 * traversal is cleared, including pathFound flag.
 *
 * @parameters: None.
 *
 * @returns: None.
 */

void SixDegrees::clear_metadata()
{
    graph.clear_metadata();
    pathFound = false;
}

/*
 * @function: bfs_function
 * @purpose: Conducts breadth-first search 
 * (BFS) traversal to find a path between two artists.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: If a path between 'from' and 
 * 'to' is found, it's printed. If not, an appropriate 
 * message is printed.
 *
 * @parameters: from - The starting artist 
 * (const Artist&), to - The target artist (const Artist&).
 *
 * @returns: None.
 */

void SixDegrees::bfs_function(const Artist &from, const Artist &to)
{
    queue<Artist> q;
    vector<Artist> neighbors;

    if (not validate_from_to(from, to)) {
        return;
    }

    q.push(from);
    while (!q.empty()) {
        Artist curArtist = q.front();
        q.pop();
        if (graph.is_marked(curArtist)) {
            continue; //discard
        }
        graph.mark_vertex(curArtist);

        if (curArtist == to) {
            print_path(from,to);
            clear_metadata();
            return;
        }
        neighbors = graph.get_vertex_neighbors(curArtist);
        for (size_t i = 0; i < neighbors.size(); i++) {
            if (graph.is_marked(neighbors[i]) == false) {
                graph.set_predecessor(neighbors[i], curArtist);
                q.push(neighbors[i]);
            }
        }
    }

    print_path(from, to);
    clear_metadata();
    return;
}

/*
 * @function: dfs_function
 * @purpose: Conducts depth-first search (DFS) 
 * traversal to find a path between two artists.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: If a path between 'from' and 
 * 'to' is found, it's printed. If not, an appropriate 
 * message is printed.
 *
 * @parameters: from - The starting artist 
 * (const Artist&), to - The target artist 
 * (const Artist&).
 *
 * @returns: None.
 */

void SixDegrees::dfs_function(const Artist &from, const Artist &to)
{
    if (not validate_from_to(from, to)) {
        return;
    }

    clear_metadata();
    Artist nullArtist = Artist();
    dfs_private(from, nullArtist, from, to);
    if (!pathFound) {
        print_path(from, to);
        return;
    }
}

/*
 * @function: not_function
 * @purpose: Finds the shortest path between two 
 * artists, excluding specified artists from consideration.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: The shortest path between 
 * 'from' and 'to' is found, excluding artists in 'notList'.
 *
 * @parameters: from - The starting artist 
 * (const Artist&), to - The target artist (const Artist&),
 *              notList - A list of artists to 
 * exclude from consideration (vector<Artist>).
 *
 * @returns: None.
 */

void SixDegrees::not_function(const Artist &from, const Artist &to, 
                                                vector<Artist> notList)
{
    clear_metadata();
    bool returnValue = validate_from_to(from, to);
    bool validateValue = validateList(notList);

    if (!returnValue or !validateValue) {
        return;
    }

    for (size_t i = 0; i < notList.size(); i++) {
        exclude(notList[i]);
    }

    bfs_function(from, to);
    clear_metadata();
}

/*
 * @function: exclude
 * @purpose: Marks an artist as excluded from 
 * the graph traversal.
 *
 * @preconditions: None.
 *
 * @postconditions: The provided artist is 
 * marked as excluded from further traversal.
 *
 * @parameters: exclude - The artist to be 
 * excluded (const Artist&).
 *
 * @returns: None.
 */

void SixDegrees::exclude(const Artist &exclude)
{
    graph.mark_vertex(exclude);
}

/*
 * @function: configureOutput
 * @purpose: Configures the output stream 
 * for printing messages.
 *
 * @preconditions: None.
 *
 * @postconditions: The 'output' stream is 
 * configured to the provided 'out' stream.
 *
 * @parameters: out - Pointer to the output 
 * stream (ostream*).
 *
 * @returns: None.
 */

void SixDegrees::configureOutput(ostream *out)
{
    output = out;
}

/*
 * @function: print_path
 * @purpose: Prints the path between two artists 
 * if it exists.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: None.
 *
 * @parameters: from - The starting artist 
 * (const Artist&), to - The target artist 
 * (const Artist&).
 *
 * @returns: None.
 */

void SixDegrees::print_path(const Artist &from, const Artist &to)
{
    stack<Artist> reportedPath = graph.report_path(from, to);
    if (reportedPath.empty()) {
        *output << "A path does not exist between "
                << "\"" << from.get_name() << "\"" << " and "
                << "\"" << to.get_name() << "\"." << endl;
        return;
    } 

    while (reportedPath.size() > 1) {
        Artist a = reportedPath.top();
        reportedPath.pop();
        Artist b = reportedPath.top();

        *output << "\"" << a.get_name() << "\"" << " collaborated with "
                << "\"" << b.get_name() << "\"" << " in "
                << "\"" << graph.get_edge(a, b) << "\"." << endl;
        
    }
    *output << "***" <<endl;
}

/*
 * @function: validate_from_to
 * @purpose: Validates if both 'from' and 'to' 
 * artists exist in the graph dataset.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: None.
 *
 * @parameters: from - The starting artist 
 * (const Artist&), to - The target artist (const Artist&).
 *
 * @returns: True if both 'from' and 'to' 
 * artists exist in the graph dataset, false otherwise.
 */

bool SixDegrees::validate_from_to(const Artist &from, const Artist &to) {

    bool returnValue = true;
    if (graph.is_vertex(from) == false) {
        *output << "\"" << from.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        returnValue = false;
    }

    if (graph.is_vertex(to) == false) {
        *output << "\"" << to.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        returnValue = false;
    }
    return returnValue;
}

/*
 * @function: validateList
 * @purpose: Validates if all artists in the 
 * provided list exist in the graph dataset.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: None.
 *
 * @parameters: List - The list of artists to 
 * validate (vector<Artist>).
 *
 * @returns: True if all artists in the list 
 * exist in the graph dataset, false otherwise.
 */

bool SixDegrees::validateList(vector<Artist> List)
{
    bool validateValue = true;
    for (size_t i = 0; i < List.size(); i++) {
        if (validateExclude(List[i]) == false) {
            validateValue = false;
        }
    }
    return validateValue;
}

/*
 * @function: validateExclude
 * @purpose: Validates if the provided artist 
 * exists in the graph dataset.
 *
 * @preconditions: The graph is initialized and 
 * contains vertices representing artists.
 *
 * @postconditions: None.
 *
 * @parameters: exclude - The artist to validate 
 * (const Artist&).
 *
 * @returns: True if the provided artist exists 
 * in the graph dataset, false otherwise.
 */

bool SixDegrees::validateExclude(const Artist &exclude)
{
    bool validateValue = true;
    if (graph.is_vertex(exclude) == false) {
        *output << "\"" << exclude.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        validateValue = false;
    }
    return validateValue;
}

/*
 * @function: dfs_private
 * @purpose: Conducts depth-first search (DFS) traversal 
 * to find a path from one artist to another within six 
 * degrees of separation.
 *
 * @preconditions: The graph is initialized and contains 
 * vertices representing artists. 'from' and 'to' artists 
 * exist in the graph.
 *
 * @postconditions: If a path from 'from' to 'to' is found 
 * within six degrees of separation, it's printed, and 
 * 'pathFound' flag is set to true.
 *
 * @parameters: curArtist - The current artist, parent - 
 * The parent artist, from - Starting artist, to - Target 
 * artist.
 */

void SixDegrees::dfs_private(const Artist &curArtist, const Artist &parent, 
                                         const Artist &from, const Artist &to)
{
    vector<Artist> neighbors;
    graph.mark_vertex(curArtist);
    if (parent.get_name() != "") {
        graph.set_predecessor(curArtist, parent);
    }
    if (curArtist == to) {
        print_path(from, to);
        pathFound = true;
        return;
    }

    neighbors = graph.get_vertex_neighbors(curArtist);
    for (size_t i = 0; i < neighbors.size(); i++) {
        if (!graph.is_marked(neighbors[i])) {
            if (!pathFound) {
                dfs_private(neighbors[i], curArtist, from, to);
            }
        }
    }
}