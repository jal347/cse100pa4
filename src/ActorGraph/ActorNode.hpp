/*
 * ActorNode.hpp
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

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Maybe include some data structures here

using namespace std;

/**
 * ActorNode class contains all the data values required to build an imdb graph
 * and traverse it using BFS, Djikstra's, and Kruskals.
 */
class ActorNode {
  public:
    // the actors name
    string name;

    // weight
    int weight = -1;

    // done?
    bool done = false;

    // movie, actor pair
    pair<string, string> previous = make_pair("", "");

    // priority
    int priority = 0;

    // for find method
    string parent;
    int rank = 0;

    // list of all the movies the actor played in
    vector<string> moviesActed;

    // movie connections
    unordered_map<string, int> connections;
    /**
     * Constuctor of the Actor Node
     */
    ActorNode(string name) : name(name), parent(name) {}
};

// comparator for ActorNodes. if priority is equal, compares names.
struct ActorNodeComp {
    bool operator()(ActorNode*& left, ActorNode*& right) const {
        if (left->priority == right->priority) {
            if (left->name.compare(right->name) < 0) {
                return true;
            } else
                return false;
        }

        return (left->priority > right->priority);
    }
};

// dj stands for DeJa vu
struct djComp {
    bool operator()(pair<string, int> left, pair<string, int> right) const {
        return (left.second > right.second);
    }
};

// basically an edge class
struct Edge {
    string src;
    string dest;
    string movie;
    int weight;
};

// Compares edges by weights to work with a priority queue
struct EdgeComp {
    bool operator()(Edge left, Edge right) {
        return (left.weight > right.weight);
    }
};

#endif  // ACTORNODE_HPP
