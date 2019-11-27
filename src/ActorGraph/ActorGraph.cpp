/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#define SECOND_LEVEL 2

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
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year),
 *                      otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

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

            // get the next string before hitting a tab character and put it in 'str'
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[2]);
        
        // TODO: we have an actor/movie relationship, now what?
	
	
	//get the realMovie name
	string realMovie = movie_title+"#@"+record[2];


	//check if there is an actornode if there isnt make a new one and shove it into the hashmap
	if(actors.find(actor) == actors.end()) {
		ActorNode* actorN = new ActorNode(actor);
		actorN->moviesActed.push_back(realMovie);
		actors[actor] = actorN;

	}
	// if the actor already exists, update his/her list of movies
	else {
		actors[actor]->moviesActed.push_back(realMovie);
	}


	//weight check
	int weight;
	if(use_weighted_edges) {
		weight = 1 + (2019 - year);
	}
	else {
		weight = 1;
	}
	
	//check if there is already a realMovie key
	if(movies.find(realMovie) == movies.end()) {
		//make a vector pointer for the list of actors
		vector<string>* actorList = new vector<string>();
		//push in the actor
		actorList->push_back(actor);
		//set the key movie and value vector
		movies[realMovie] = actorList;

		//set movieWeight
		movieWeight[realMovie] = weight;
	}
	//if there is already a realMovie key dont need to set weight
	else{
		//find the vector and put actor into the actorlist
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


void ActorGraph::connect() {
    //fill number of connections for current actor
    for(auto curr = actors.begin(); curr != actors.end(); curr++) {
	ActorNode * currNode = curr->second;
	//go through all the movies this actor acted in
    	for(auto movieEdge: currNode->moviesActed) {
		//go through all the actors in that movie
		for(auto j: *(movies[movieEdge])){
			//if the actor name is not itself we add connection
			if(currNode != actors[j]) {
				//if there isnt an actor key then make new connection
				if(currNode->connections.find(j) == currNode->connections.end()) {
					currNode->connections[j] = 1;
				}
				//increment the connection
				else {
					currNode->connections[j] = currNode->connections[j] + 1;
				}
			}		
		}
	}
    }

}

void ActorGraph::BFS(string actor1, string actor2){
	//reset all of the actornodes before running bfs algorithm
	for(auto it = actors.begin(); it != actors.end(); it++) {
		it->second->weight = -1;
		it->second->previous = pair<string,string>();
	}

	list<string> queue;

	cout << "Computing path for (" << actor1 << ") -> (" << actor2 << ")" << endl;


	
	ActorNode * actor = actors[actor1];
	//add weight to signify its visited
	actor->weight = actor->weight + 1;

	//push name of actor into the queue
	queue.push_back(actor->name);
	
	// run BFS until we realize the actors can't connect (edge case)
	while(!queue.empty()) {
		//get the actor from front of queue
		string curr = queue.front();
		//pop it
		queue.pop_front();
		// pull actornode from hash map using name of actor
		ActorNode * currNode = actors[curr];

		//get all of the adj actorNodes. movieEdge is a string movie@#year
		for(auto movieEdge : currNode->moviesActed) {
			// j is a String: the name of an actor was in the movie
			for(auto j: *(movies[movieEdge])) {
				// pull actornode out of hashmap of actors using name as key
				ActorNode * adjActor = actors[j];
				//if not visited and not the same actor then update weight, previous and enqueue
				if(adjActor->weight == -1 && adjActor != currNode) {
					//update weight shows that visited
					adjActor->weight = currNode->weight + 1;
					//update previous make a pair movieName, actorname
					adjActor->previous = make_pair(movieEdge, currNode->name);
					//enqueue
					queue.push_back(adjActor->name);
					if(adjActor == actors[actor2]){
						return;
					}
				}
			}
		}

	}
	return;
	// if the queue is empty, we didn't find a connection
	/*
	if(queue.size() == 0){
		cout << "were returnin lol" << endl;
		return;
	}*/	
	
}

//fills collab1 and collab2 with strings based on the query node
void ActorGraph::fillCandidates(string q) {
	//reset candidates vectors
	collab1.clear();
	collab2.clear();

	//reset all of the actornodes before running bfs algorithm
	for(auto it = actors.begin(); it != actors.end(); it++) {
		it->second->weight = -1;
		it->second->previous = pair<string,string>();
		it->second->priority = 0;
		it->second->connections.clear();
	}

	
	list<string> queue;

	ActorNode * actor = actors[q];
	//add weight to signify its visited
	actor->weight = actor->weight + 1;

	//push name of actor into the queue
	queue.push_back(actor->name);


	// run BFS until we run pop an actor with weight greater than 1
	while(!queue.empty()) {
		//get the actor from front of queue
		string curr = queue.front();
		//pop it
		queue.pop_front();
		// pull actornode from hash map using name of actor
		ActorNode * currNode = actors[curr];

		//before running bfs check if actor weight is greater than 1 if so return
		if(currNode->weight > 1) {
			return;
		}

		//get all of the adj actorNodes. movieEdge is a string movie@#year
		for(auto movieEdge : currNode->moviesActed) {
			// j is a String: the name of an actor was in the movie
			for(auto j: *(movies[movieEdge])) {
				// pull actornode out of hashmap of actors using name as key
				ActorNode * adjActor = actors[j];
				
				
				//if not the same actor then we make connection
				if(adjActor != currNode) {
					//if there isnt an actor key then make new connection
					if(currNode->connections.find(j) == currNode->connections.end()) {
						currNode->connections[j] = 1;
					}
					//increment the connection
					else {
						currNode->connections[j] = currNode->connections[j] + 1;
					}

				} 

				//if not visited and not the same actor then update weight, previous and enqueue
				if(adjActor->weight == -1 && adjActor != currNode) {
					//update weight shows that visited
					adjActor->weight = currNode->weight + 1;
					//update previous make a pair movieName, actorname
					adjActor->previous = make_pair(movieEdge, currNode->name);
					//enqueue
					queue.push_back(adjActor->name);
					//check if actor weight is 1 then put into collab1
					if(adjActor->weight == 1){
						collab1.push_back(adjActor->name);
					}

					//check if actor weight is 2 then put into collab2
					if(adjActor->weight == SECOND_LEVEL){
						collab2.push_back(adjActor->name);
					}

				}
			}
		}

	}
}


void ActorGraph::closureCount(string q) {
	//for the first group determine priority of each candidate
	for(auto cand : collab1) {
		//grab candidate from hashmap
		ActorNode * candActor = actors[cand];
		//now calculate number of triadic closure
		//go through each movie the query actor is in
		for(auto movieEdge : actors[q]->moviesActed) {
			//for each cast member of this movie (who is not the candidate nor the query actor
			for (auto j: *(movies[movieEdge])) {
				//who is not candidate nor query actor
				if(actors[cand] != actors[j] && actors[q] != actors[j]) {
					if(candActor->connections.find(j) != candActor->connections.end()) {
						//increment priority
						candActor->priority += candActor->connections[j];
					}
				}
			}
		}
	}

	//go through collab 1 and shove into priority queue
	for(unsigned int i = 0; i < collab1.size(); i++) {
		if(pq1.size() >= 4) {
			pq1.push(actors[collab1[i]]);
			pq1.pop();
		}
		else {
			pq1.push(actors[collab1[i]]);
		}
	}

	//for the second group determine priority of each candidate
	for(auto cand : collab2) {
		//grab candidate from hashmap
		ActorNode * candActor = actors[cand];
		//now calculate number of triadic closure
		//go through each movie the query actor is in
		for(auto movieEdge : actors[q]->moviesActed) {
			//for each cast member of this movie (who is not the candidate nor the query actor
			for (auto j: *(movies[movieEdge])) {
				//who is not candidate nor query actor
				if(candActor != actors[j] && actors[q] != actors[j]) {
					// if there's a connection between the queue node and the candidate
					if(actors[j]->connections.find(cand) != actors[j]->connections.end()) {
						//increment priority by number of connections
						candActor->priority += actors[j]->connections[cand];
					}
				}
			}
		}
	}

	//go through collab 2 and shove into priority queue
	for(unsigned int i = 0; i < collab2.size(); i++) {
		if(pq2.size() >= 4) {
			pq2.push(actors[collab2[i]]);
			pq2.pop();
		}
		else {
			pq2.push(actors[collab2[i]]);
		}
	}

	
}
