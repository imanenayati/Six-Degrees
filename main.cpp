/*
 * main.cpp
 *
 * CS 15 Project 2: Six Degrees of Collaboration
 * 
 * by Iman Enayati, April 18th 2024
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>

#include "Artist.h"
#include "CollabGraph.h"
#include "SixDegrees.h"

using namespace std;

void handleLine(string line, 
                vector<string> &commands, 
                vector<unique_ptr<Artist>> &notList,
                unique_ptr<SixDegrees> &sixDegrees)
{
    if (line == "quit") {
        exit(0);
    }

    bool skipNot = false;
    if (commands.size() < 3) {
        if (
        commands.size() == 0 && !(
        line == "bfs" || line == "dfs" || line == "not")) {
*(sixDegrees->output) << line << " is not a command. Please try again." << endl;
        return;
        }
        if (commands.size() == 2) {
            skipNot = true;
        }
        commands.push_back(line);
    }

    if (commands.size() != 3) {
        return;
    }

    Artist s = Artist(commands[1]);
    Artist f = Artist(commands[2]);

    if (commands.front() == "bfs") {
        sixDegrees->clear_metadata(); 
        sixDegrees->bfs_function(s, f);
        commands.clear();
        return;
    }

    if (commands.front() == "dfs") {
        sixDegrees->clear_metadata(); 
        sixDegrees->dfs_function(s, f);
        commands.clear();
        return;
    }

    if (line == "*") {
        vector<Artist> plainNotList;
        for (auto &uniqueArt : notList) {
            plainNotList.push_back(*uniqueArt);
        }
        sixDegrees->not_function(s, f, plainNotList);
        commands.clear();
        notList.clear();
        return;
    } else {
        if (!skipNot) {
            notList.push_back(make_unique<Artist>(line));
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 4) {
        cerr<<"Usage: ./SixDegrees dataFile [commandFile] [outputFile]"<<endl;
        exit(0);
    }

    ifstream dataFile(argv[1]);
    string line;
    vector<unique_ptr<Artist>> artistList;
    unique_ptr<SixDegrees> sixDegree = make_unique<SixDegrees>();

    if (dataFile.is_open()) {
        unique_ptr<Artist> curArtist;
        bool nameSet = false;
        while (getline(dataFile, line)) {
            if (line != "*") {
                if (!nameSet) {
                    curArtist = make_unique<Artist>();
                    curArtist->set_name(line);
                    nameSet = true;
                } else {
                    curArtist->add_song(line);
                }
            } else {
                nameSet = false;
                artistList.push_back(std::move(curArtist));
                curArtist = make_unique<Artist>();
            }
        }
        if (nameSet) {
            artistList.push_back(std::move(curArtist));
        }

        vector<Artist*> rawList;
        for (auto &artPtr : artistList) {
            rawList.push_back(artPtr.get());
        }
        sixDegree->populateGraph(rawList);

    } else {
        cerr << "Error: could not open file " << argv[1] << endl;
        exit(0);
    }

    ofstream outputFile;
    if (argc == 4) {
        outputFile.open(argv[3]);
        sixDegree->configureOutput(&outputFile);
    }

    vector<string> commands;
    vector<unique_ptr<Artist>> notList;

    if (argc >= 3) {
        ifstream commandFile(argv[2]);
        if (commandFile.is_open()) {
            while (getline(commandFile, line)) {
                handleLine(line, commands, notList, sixDegree);
            }
            commandFile.close();
        } else {
            cerr << "Error: could not open file " << argv[2] << endl;
            exit(0);
        }

    } else {
        while (getline(cin, line)) {
            handleLine(line, commands, notList, sixDegree);
        }
    }
    
    outputFile.close();
    return 0;
}