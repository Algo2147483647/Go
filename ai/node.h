#include <unordered_map>
#include "./core/Go.h"

namespace GoAI { 
	/*
	 *  Node of Monte-Carlo Tree
	 */
	class Node {
	public:
		// evalution of node
		int value = 0;
		int visit = 0;

		Go::State* state = NULL;
		Node* parent = NULL;

		// actions & next states
		vector<int> actionSet;
		unordered_map<int, Node*> child;  

		// Constructor
		Node(Go::State* _state, Node* _parent) : state(_state), parent(_parent){
			generateActionSet();
		}

		~Node() {
			// delete all child node except the best node
			for (auto e = child.begin(); e != child.end(); e++) {
				if (e->second == NULL)
					continue;
				delete e->second;
			}
			delete state;
		}

		// generate action set randomly
		void generateActionSet() {
			actionSet.push_back(Go::PASS);
			for (int i = 0; i < Go::BOARD_COLOR_NUM; i++)
				if (state->mark[i] == -1)
					actionSet.push_back(i);
		}
	};
}