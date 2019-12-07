/*
 * ActorGraph.hpp
 * Author: Felix Chu fhchu@ucsd.edu
 *         Jason Lin jal347@ucsd.edu
 * Date:   12-06-2019
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 *
 * Sources: class notes, discussion slides
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <queue>
#include "ActorNode.hpp"

// Maybe include some data structures here

using namespace std;

/**
 * ActorGraph class contains the methods and variables to load imdb data from
 * file, create a graph, then run through the graph with variable algorithms
 * in order to achieve different goals.
 */

class ActorGraph {
  protected:
    // Maybe add class data structure(s) here

    // list of first degree neighbors of query actor/actress
    vector<string> collab1;
    // list of second degree neighbors of query actor/actress
    vector<string> collab2;

    // these two hashmaps are basically a fake movie class
    // hashmap of all the movies. key: movie#@year value: list of actor names in
    // that movie
    unordered_map<string, vector<string>*> movies;
    // hashmap of of all movies and their weights (inverse year)
    unordered_map<string, int> movieWeight;

  public:
    // hashmap of all the actors pointers like how we kept leaves in huffman
    // encoding
    unordered_map<string, ActorNode*> actors;
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    // priority queues containing the closures of all the actors
    priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> pq1;
    priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> pq2;

    // Maybe add some more methods here

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 -
     * movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */

    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /* iterate through the actor-movie graph with a breadth first search to find
       shortest path */
    void BFS(string actor1, string actor2);

    /* runs djikstra's shortest path algorithm on the weighted graph */
    void shortestPath(string actor1, string actor2);

    /* Calculates all the possible closures (for collaborated and uncollaborated
   and puts them into a pq to grab the best ones later */
    void closureCount(string q);

    // fills collab1 and collab2 with strings based on the query node
    void fillCandidates(string q);

    /* runs kruskal's and union find algorithm to create a minimal spanning tree
     */
    void kruskals(ostream& out);
    /* finds two nodes and combines them under one root */
    void Union(string a, string b);
    /* finds the root of a node, doing path compression along the way */
    string find(string s);
    // helper method to connect all edges
    void connectEdge();
    /* deconstructor for ActorGraph; frees up memory from heap by iterating
       through hashmaps that may have data in them */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
