#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "ActorGraph.cpp"
#include "ActorNode.hpp"

using namespace std;
using namespace testing;



TEST(SimpleActorNode, TEST_ACTOR) {
	ActorNode a("felix");    	
	ASSERT_EQ(a.name, "felix");
}

TEST(SimpleActorNode, TEST_WEIGHT) {
	ActorNode a("felix");	
	ASSERT_EQ(a.priority, 0);
	ASSERT_EQ(a.weight, -1);
}

TEST(SimpleActorNode, TEST_MOVIES) {
	ActorNode a("felix");
	a.moviesActed.push_back("CSE100");	
	ASSERT_EQ(a.moviesActed[0], "CSE100");
}


TEST(SimpleActorNode, TEST_PAIR) {
	ActorNode a("felix");
	a.previous = make_pair("CSE100" , "Jason");
	pair<string, string> p = make_pair("CSE100", "Jason");	
	ASSERT_EQ(a.previous, p);
}

TEST(SimpleActorNode, TEST_BS){
	ActorNode b("lol");
	ActorNode a("fuck");
	ActorNode c("omg");
	ActorNode d("alpha");
	c.priority = 2;
    	priority_queue<ActorNode*, vector<ActorNode*>, ActorNodeComp> pq1;
	pq1.push(&a);
	pq1.push(&b);
	pq1.push(&c);
	pq1.push(&d);
	ActorGraph graph;
	graph.loadFromFile("../data/imdb_small_sample.tsv", false);
	graph.BFS("Robert Downey Jr.", "Kevin Bacon");
	graph.fillCandidates("Michael Fassbender");
	graph.closureCount("Michael Fassbender");
}

