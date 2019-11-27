/*
 * ActorNode.hpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

// Maybe include some data structures here

using namespace std;

/**
 * TODO: add class header
 */
class ActorNode {
  


  public:
  	
	//the actors name
	string name;

	//weight
	int weight = -1;

	//movie, actor pair
	pair<string, string> previous = make_pair("", "");

	//priority
	int priority;	


	//list of all the movies the actor played in 
	vector<string> moviesActed;
	
	//movie connections
	unordered_map<string, int> connections;

    	/**
     	* Constuctor of the Actor Node
     	*/
    	ActorNode(string name):name(name){}

    
};


struct ActorNodeComp{
    bool operator()(ActorNode*& left, ActorNode*& right) const{
	    if(left->priority == right->priority){
	    	if(left->name.compare(right->name) < 0){
			return true;
		}
		else return false;
	    }

	    return(left->priority > right->priority);
    }
};



#endif  // ACTORNODE_HPP
