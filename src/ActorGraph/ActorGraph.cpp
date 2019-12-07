/*
 * ActorGraph.cpp
 * Author: Felix Chu fhchu@ucsd.edu
 *         Jason Lin jal347@ucsd.edu
 * Date:   12-06-2019
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in imdb_2019.tsv. Feel free to modify
 * any/all aspects as you wish.
 *
 * Sources: class notes, discussion slides
 */

#include "ActorGraph.hpp"
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#define SECOND_LEVEL 2
#define NUM_COLS 3
#define YEAR_COL 2
#define CURRENT_YEAR 2019
#define NUM_COLLABS 4
using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {}

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
bool ActorGraph::loadFromFile(const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        if (record.size() != NUM_COLS) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[YEAR_COL]);

        // get the realMovie name
        string realMovie = movie_title + "#@" + record[YEAR_COL];

        // check if there is an actornode if there isnt make a new one and shove
        // it into the hashmap
        if (actors.find(actor) == actors.end()) {
            ActorNode* actorN = new ActorNode(actor);
            actorN->moviesActed.push_back(realMovie);
            actors[actor] = actorN;

        }
        // if the actor already exists, update his/her list of movies
        else {
            actors[actor]->moviesActed.push_back(realMovie);
        }

        // weight check
        int weight;
        if (use_weighted_edges) {
            weight = 1 + (CURRENT_YEAR - year);
        } else {
            weight = 1;
        }

        // check if there is already a realMovie key
        if (movies.find(realMovie) == movies.end()) {
            // make a vector pointer for the list of actors
            vector<string>* actorList = new vector<string>();
            // push in the actor
            actorList->push_back(actor);
            // set the key movie and value vector
            movies[realMovie] = actorList;

            // set movieWeight
            movieWeight[realMovie] = weight;
        }
        // if there is already a realMovie key dont need to set weight
        else {
            // find the vector and put actor into the actorlist
            movies.at(realMovie)->push_back(actor);
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
/* deconstructor for ActorGraph; frees up memory from heap by iterating through
   hashmaps that may have data in them */
ActorGraph::~ActorGraph() {
    // run through the hashmap of all the actors and free memory
    for (auto it = actors.begin(); it != actors.end(); it++) {
        delete (it->second);
    }

    // run through the hashmap of all the movies and free memory
    for (auto it = movies.begin(); it != movies.end(); it++) {
        delete (it->second);
    }
}
// helper method to connect all edges
void ActorGraph::connectEdge() {
    // fill number of connections for current actor
    for (auto curr = actors.begin(); curr != actors.end(); curr++) {
        ActorNode* currNode = curr->second;
        // go through all the movies this actor acted in
        for (auto movieEdge : currNode->moviesActed) {
            // go through all the actors in that movie
            for (auto j : *(movies[movieEdge])) {
                // if the actor name is not itself we add connection
                if (currNode != actors[j]) {
                    // if there isnt an actor key then make new connection
                    if (currNode->connections.find(j) ==
                        currNode->connections.end()) {
                        currNode->connections[j] = 1;
                    }
                    // increment the connection
                    else {
                        currNode->connections[j] = currNode->connections[j] + 1;
                    }
                }
            }
        }
    }
}

/* iterate through the actor-movie graph with a breadth first search to find
   shortest path */
void ActorGraph::BFS(string actor1, string actor2) {
    // reset all of the actornodes before running bfs algorithm
    for (auto it = actors.begin(); it != actors.end(); it++) {
        it->second->weight = -1;
        it->second->previous = pair<string, string>();
    }

    list<string> queue;

    ActorNode* actor = actors[actor1];
    // add weight to signify its visited
    actor->weight = actor->weight + 1;

    // push name of actor into the queue
    queue.push_back(actor->name);

    // run BFS until we realize the actors can't connect (edge case)
    while (!queue.empty()) {
        // get the actor from front of queue
        string curr = queue.front();
        // pop it
        queue.pop_front();
        // pull actornode from hash map using name of actor
        ActorNode* currNode = actors[curr];

        // get all of the adj actorNodes. movieEdge is a string movie@#year
        for (auto movieEdge : currNode->moviesActed) {
            // j is a String: the name of an actor was in the movie
            for (auto j : *(movies[movieEdge])) {
                // pull actornode out of hashmap of actors using name as key
                ActorNode* adjActor = actors[j];
                // if not visited and not the same actor then update weight,
                // previous and enqueue
                if (adjActor->weight == -1 && adjActor != currNode) {
                    // update weight shows that visited
                    adjActor->weight = currNode->weight + 1;
                    // update previous make a pair movieName, actorname
                    adjActor->previous = make_pair(movieEdge, currNode->name);
                    // enqueue
                    queue.push_back(adjActor->name);
                    if (adjActor == actors[actor2]) {
                        return;
                    }
                }
            }
        }
    }
    return;
}

// fills collab1 and collab2 with strings based on the query node
void ActorGraph::fillCandidates(string q) {
    // reset candidates vectors
    collab1.clear();
    collab2.clear();

    // reset all of the actornodes before running bfs algorithm
    for (auto it = actors.begin(); it != actors.end(); it++) {
        it->second->weight = -1;
        it->second->previous = pair<string, string>();
        it->second->priority = 0;
        it->second->connections.clear();
    }

    list<string> queue;

    ActorNode* actor = actors[q];
    // add weight to signify its visited
    actor->weight = actor->weight + 1;

    // push name of actor into the queue
    queue.push_back(actor->name);

    // run BFS until we run pop an actor with weight greater than 1
    while (!queue.empty()) {
        // get the actor from front of queue
        string curr = queue.front();
        // pop it
        queue.pop_front();
        // pull actornode from hash map using name of actor
        ActorNode* currNode = actors[curr];

        // before running bfs check if actor weight is greater than 1 if so
        // return
        if (currNode->weight > 1) {
            return;
        }

        // get all of the adj actorNodes. movieEdge is a string movie@#year
        for (auto movieEdge : currNode->moviesActed) {
            // j is a String: the name of an actor was in the movie
            for (auto j : *(movies[movieEdge])) {
                // pull actornode out of hashmap of actors using name as key
                ActorNode* adjActor = actors[j];

                // if not the same actor then we make connection
                if (adjActor != currNode) {
                    // if there isnt an actor key then make new connection
                    if (currNode->connections.find(j) ==
                        currNode->connections.end()) {
                        currNode->connections[j] = 1;
                    }
                    // increment the connection
                    else {
                        currNode->connections[j] = currNode->connections[j] + 1;
                    }
                }

                // if not visited and not the same actor then update weight,
                // previous and enqueue
                if (adjActor->weight == -1 && adjActor != currNode) {
                    // update weight shows that visited
                    adjActor->weight = currNode->weight + 1;
                    // update previous make a pair movieName, actorname
                    adjActor->previous = make_pair(movieEdge, currNode->name);
                    // enqueue
                    queue.push_back(adjActor->name);
                    // check if actor weight is 1 then put into collab1
                    if (adjActor->weight == 1) {
                        collab1.push_back(adjActor->name);
                    }

                    // check if actor weight is 2 then put into collab2
                    if (adjActor->weight == SECOND_LEVEL) {
                        collab2.push_back(adjActor->name);
                    }
                }
            }
        }
    }
}

/* Calculates all the possible closures (for collaborated and uncollaborated
   and puts them into a pq to grab the best ones later */
void ActorGraph::closureCount(string q) {
    // for the first group determine priority of each candidate
    for (auto cand : collab1) {
        // grab candidate from hashmap
        ActorNode* candActor = actors[cand];
        // now calculate number of triadic closure
        // go through each movie the query actor is in
        for (auto movieEdge : actors[q]->moviesActed) {
            // for each cast member of this movie (who is not the candidate nor
            // the query actor
            for (auto j : *(movies[movieEdge])) {
                // who is not candidate nor query actor
                if (actors[cand] != actors[j] && actors[q] != actors[j]) {
                    if (candActor->connections.find(j) !=
                        candActor->connections.end()) {
                        // increment priority
                        candActor->priority += candActor->connections[j];
                    }
                }
            }
        }
    }

    // go through collab 1 and shove into priority queue
    for (unsigned int i = 0; i < collab1.size(); i++) {
        if (pq1.size() >= NUM_COLLABS) {
            pq1.push(actors[collab1[i]]);
            pq1.pop();
        } else {
            pq1.push(actors[collab1[i]]);
        }
    }

    // for the second group determine priority of each candidate
    for (auto cand : collab2) {
        // grab candidate from hashmap
        ActorNode* candActor = actors[cand];
        // now calculate number of triadic closure
        // go through each movie the query actor is in
        for (auto movieEdge : actors[q]->moviesActed) {
            // for each cast member of this movie (who is not the candidate nor
            // the query actor
            for (auto j : *(movies[movieEdge])) {
                // who is not candidate nor query actor
                if (candActor != actors[j] && actors[q] != actors[j]) {
                    // if there's a connection between the queue node and the
                    // candidate
                    if (actors[j]->connections.find(cand) !=
                        actors[j]->connections.end()) {
                        // increment priority by number of connections
                        candActor->priority += actors[j]->connections[cand];
                    }
                }
            }
        }
    }

    // go through collab 2 and shove into priority queue
    for (unsigned int i = 0; i < collab2.size(); i++) {
        if (pq2.size() >= NUM_COLLABS) {
            pq2.push(actors[collab2[i]]);
            pq2.pop();
        } else {
            pq2.push(actors[collab2[i]]);
        }
    }
}
/* runs djikstra's shortest path algorithm on the weighted graph */
void ActorGraph::shortestPath(string actor1, string actor2) {
    // reset all of the actornodes before running shortest path algorithm
    for (auto it = actors.begin(); it != actors.end(); it++) {
        it->second->weight = INT_MAX;
        it->second->previous = pair<string, string>();
        it->second->done = false;
    }

    // a pointer to actor (actor name) and a path cost pair
    priority_queue<pair<string, int>, vector<pair<string, int>>, djComp> pq;

    // starting actor (S)
    ActorNode* start = actors[actor1];
    // set S's weight(dist) to 0
    start->weight = 0;

    // enqueue {S, 0} onto PQ
    pq.push(make_pair(start->name, start->weight));

    // set a bool found value to cut the shortest path algorithm
    bool found = false;

    while (!pq.empty() && !found) {
        // dequeueing node from front of q
        pair<string, int> curr = pq.top();
        pq.pop();
        ActorNode* currNode = actors[curr.first];
        // check if we found the search term
        if (curr.first == actor2) {
            found = true;
        }

        // if node is not done
        if (currNode->done == false) {
            // set done as true
            currNode->done = true;

            // get all of the adj actorNodes. movieEdge is a string movie@#year
            for (auto movieEdge : currNode->moviesActed) {
                // get the edgeWeight
                int edgeWeight = movieWeight[movieEdge];
                // j is a String: the name of an actor was in the movie
                for (auto j : *(movies[movieEdge])) {
                    // pull actornode out of hashmap of actors using name as key
                    ActorNode* adjActor = actors[j];
                    // if not the same actor then update weight, previous and
                    // enqueue
                    if (adjActor != currNode) {
                        // get distance from currnode to adj node
                        int compare = currNode->weight + edgeWeight;
                        // if c is less than adj->weight
                        if (adjActor->weight > compare) {
                            // set previous
                            adjActor->previous =
                                make_pair(movieEdge, currNode->name);
                            // set distance
                            adjActor->weight = compare;
                            // enqueue into the pq
                            pq.push(
                                make_pair(adjActor->name, adjActor->weight));
                        }
                    }
                }
            }
        }
    }
    return;
}

/* finds the root of a node, doing path compression along the way */
string ActorGraph::find(string s) {
    // finds the root and makes the root as parent of s
    if (actors[s]->parent != s) {
        actors[s]->parent = find(actors[s]->parent);
    }
    return actors[s]->parent;
}

/* finds two nodes and combines them under one root */
void ActorGraph::Union(string a, string b) {
    string rootA = find(a);
    string rootB = find(b);

    // union by the rank
    if (actors[a]->rank < actors[b]->rank) {
        actors[a]->parent = rootB;
    } else if (actors[a]->rank > actors[b]->rank) {
        actors[b]->parent = rootA;
    } else {
        actors[b]->parent = rootA;
        actors[a]->rank++;
    }
}
/* runs kruskal's and union find algorithm to create a minimal spanning tree */
void ActorGraph::kruskals(ostream& out) {
    Edge* result = new Edge[actors.size()];
    // count to keep track of where in the results
    unsigned int count = 0;
    unsigned int totalWeights = 0;
    // priority queue to hold all the edges
    priority_queue<Edge, vector<Edge>, EdgeComp> pq;
    // manually connect all the edges

    // manually get all edges and shove into pq
    for (auto curr = actors.begin(); curr != actors.end(); curr++) {
        ActorNode* currNode = curr->second;
        // go through all the movies this actor acted in
        for (auto movieEdge : currNode->moviesActed) {
            // go through all the actors in that movie
            for (auto j : *(movies[movieEdge])) {
                // if the actor name is not itself we add edge
                if (currNode != actors[j]) {
                    // src, des, movie name, weight
                    Edge e = {currNode->name, j, movieEdge,
                              movieWeight[movieEdge]};
                    pq.push(e);
                }
            }
        }
    }

    // run union find on the pq
    while (count < actors.size() - 1 && !pq.empty()) {
        Edge smallestE = pq.top();
        pq.pop();

        string a = find(smallestE.src);
        string b = find(smallestE.dest);
        // cycle check
        if (a.compare(b) != 0) {
            totalWeights += smallestE.weight;
            *(result + count) = smallestE;
            count++;
            Union(a, b);
        }
    }
    // print out results to file
    for (unsigned int i = 0; i < actors.size() - 1; i++) {
        Edge curr = *(result + i);
        out << "(" << curr.src << ")<--[" << curr.movie << "]-->(" << curr.dest
            << ")" << endl;
    }
    out << "#NODE CONNECTED: " << actors.size() << endl;
    out << "#EDGE CHOSEN: " << actors.size() - 1 << endl;
    out << "TOTAL EDGE WEIGHTS: " << totalWeights;
    delete[] result;
}
