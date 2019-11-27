/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>


#include "ActorGraph.cpp"

#define UNWEIGHTED "u"
#define WEIGHTED "w"
#define ARGCOUNT 5
#define WEIGHTARG 2
#define PAIRSARG 3
#define OUTARG 4
/* TODO: Main program that runs the pathfinder */


int main(int argc, char* argv[]) {
	ActorGraph graph;

	if(argc != ARGCOUNT){
		cerr << argv[0] << " called with incorrect arguments." << endl;
		return -1;
	}	
	string weightString = argv[WEIGHTARG];
	bool isWeighted = true;
	if(weightString != UNWEIGHTED && weightString != WEIGHTED){
		cerr << "wrong parameter " << argv[WEIGHTARG] << ", must be u or w" << endl;
	}
	else if (weightString == UNWEIGHTED){
		isWeighted = false;	
	}
	
	//load from file
	graph.loadFromFile(argv[1], isWeighted); 

    	if(!isWeighted){

	    // Initialize the file stream
	    ifstream infile(argv[PAIRSARG]);

	    bool have_header = false;

	    // Initialize the output stream
		ofstream outFile;
		outFile.open(argv[OUTARG], ios::out);
		outFile << "(actor)--[movie#@year]-->(actor)--..." << endl;
	    // keep reading lines until the end of file is reached
	    ActorNode* actor;
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

		if (record.size() != 2) {
		    // we should have exactly 2 columns
		    continue;
		}

		string actor1(record[0]);
		string actor2(record[1]);


		//check if first actor is there
		if(graph.actors.find(actor1) == graph.actors.end()) {
			cerr << "Failed to locate node '" << actor1 << "'" << endl;
			outFile << endl;
			continue;
		}

		//check if second actor is there
		if(graph.actors.find(actor2) == graph.actors.end()) {
			cerr << "Failed to locate node '" << actor2 << "'" << endl;
			outFile << endl;
			continue;
		}
		graph.BFS(actor1, actor2);

		// we found the connection so now we output it
		vector<string> outputList;
		// start at the last actor
		actor = graph.actors[actor2];
		// add the final actor to the string
		outputList.push_back("("+actor2+")\n");
		while(actor->previous.first != ""){
			outputList.push_back("(" + actor->previous.second + ")--[" + 
						actor->previous.first +"]-->");
			actor = graph.actors[actor->previous.second];
		}
		// reverse list and write to file
		reverse(outputList.begin(), outputList.end());
		for(unsigned int i = 0; i < outputList.size(); i++){
			outFile << outputList[i];
		}
 
	    }
	}

	
/*	
	if(!isWeighted){
		graph.Djikstra(argv[pairsArg], argv[outArg]);
	}	
*/
	}
