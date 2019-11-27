/*
 * ActorGraph.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include "ActorNode.hpp"
#include <iostream>
#include <queue>

// Maybe include some data structures here

using namespace std;

/**
 * TODO: add class header
 */


class ActorGraph {
  protected:
    // Maybe add class data structure(s) here
    
    //list of first degree neighbors of query actor/actress
    vector<string> collab1;
    //list of second degree neighbors of query actor/actress
    vector<string> collab2;

    
    
    //these two hashmaps are basically a fake movie class
    // hashmap of all the movies. key: movie#@year value: list of actor names in that movie
    unordered_map<string, vector<string>* > movies;
    //hashmap of movie and weight
    unordered_map<string, int> movieWeight;



  public:
    //hashmap of all the actors pointers like how we kept leaves in huffman encoding
    unordered_map<string, ActorNode*> actors;
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);
	
    
    //priority q
    priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> pq1;
    priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> pq2;



    // Maybe add some more methods here
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year), 
     *                      otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */

 
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    
    void BFS(string actor1, string actor2);

    void closureCount(string q);

    void fillCandidates(string q);
    
    void connect();
};

#endif  // ACTORGRAPH_HPP
