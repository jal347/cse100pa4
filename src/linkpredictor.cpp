/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>


#include "ActorGraph.cpp"

#define ARGCOUNT 5
#define QUERIES 2
#define COLLAB 3
#define UNCOLLAB 4
/* TODO: Main program that runs the linkpredictor */


int main(int argc, char* argv[]) {
	ActorGraph graph;
	if(argc != ARGCOUNT){
		cerr << argv[0] << " called with incorrect arguments." << endl;
		return -1;
	}

	// build unweighted graph
	graph.loadFromFile(argv[1], false);
	
	// intialize outstreams
	ofstream collabOut;
	collabOut.open(argv[COLLAB]);

	ofstream uncollabOut;
	uncollabOut.open(argv[UNCOLLAB]);
	collabOut << "Actor1,Actor2,Actor3,Actor4" << endl;
	uncollabOut << "Actor1,Actor2,Actor3,Actor4" << endl;
	
    // Initialize the file stream
    ifstream infile(argv[QUERIES]);

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

        if (record.size() != 1) {
            // we should have exactly 1 column
            continue;
        }

        string actor(record[0]);
	cout << "Computing predictions for (" << actor << ")" << endl;
	graph.connect();	
	graph.fillCandidates(actor);
	graph.closureCount(actor);
	vector<string> output;
	while(!graph.pq1.empty()){
		output.push_back(graph.pq1.top()->name + "\t"); 
		//collabOut << graph.pq1.top()->name << "\t";
		graph.pq1.pop();
	}
	reverse(output.begin(), output.end());
	for(auto i: output){
		collabOut << i;
	}
	collabOut << endl;
	output.clear();
	while(!graph.pq2.empty()){
		
		output.push_back(graph.pq2.top()->name + "\t"); 
		//uncollabOut << graph.pq2.top()->name << "\t";
		graph.pq2.pop();
	}
	reverse(output.begin(), output.end());
	for (auto i: output){
		uncollabOut << i;
	}
	uncollabOut << endl;
    }


	






	collabOut.close();
	uncollabOut.close();
}
