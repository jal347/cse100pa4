/**
 * Felix Chu fhchu@ucsd.edu
 * Jason Lin jal347@ucsd.edu
 *
 * Decompresses a compressed file either with true compression or ascii human-
 * readable compression
 *
 * Sources: Class notes, 
 */

#include <fstream>
#include <iostream>
#include "ActorGraph.cpp"

#define ARGCOUNT 3
#define OUTARG 2

using namespace std;

/* Main program that runs the movietraveler */
int main(int argc, char* argv[]) {
    ActorGraph graph;

    // return immediately if we don't have the right number of arguments
    if (argc != ARGCOUNT) {
        cerr << argv[0] << " called with incorrect arguments." << endl;
        return -1;
    }

    // build all the graphs from the file
    graph.loadFromFile(argv[1], true);

    // Initialize the output stream
    ofstream outFile;
    outFile.open(argv[OUTARG], ios::out);
    outFile << "(actor)<--[movie#@year]-->(actor)" << endl;

    // construct the data with a kruskal's algorithm and output to file
    graph.kruskals(outFile);
}
