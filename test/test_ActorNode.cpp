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


