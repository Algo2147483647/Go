#ifndef GO_AI_MULTITHREAD_H
#define GO_AI_MULTITHREAD_H

#include <random>
#include <algorithm>
#include <thread>
#include "./core/Go.h"
#include "thread_pool.h"
#include "node.h"

#define BANPOINT 0x7FFF
#define EYEPOINT 0x7FFC
#define THREADNUM 20

using namespace Go;

namespace GoAI { 
	inline mutex mutex_;
	static int move_pos = -1;
	static int evaluate_fg = 0;
	static vector<double> evaluate_result{ BOARD_COLOR_NUM };
	static vector<int> evaluate_visit{ BOARD_COLOR_NUM };

	Node* Select(Node* node, bool isExplore);
	void ExpandSimulate(Node* nd, int id);
	int  Simulate(Go::State& s);
	void move(Node*& root, int a);

	/*
	 *  Monte-Carlo Tree Search
	 */
	inline void MonteCarloTreeSearch(Go::State* s) {
		Node* root = new Node(s, NULL);
		ThreadPool thread_pool(THREADNUM);

		while (true) {
			// API for outside
			if (move_pos != -1) {
				move(root, move_pos);
				move_pos = -1;
			}
			if (evaluate_fg == 1) {
				evaluate_fg = 0;

				fill(evaluate_result.begin(), evaluate_result.end(), 0);
				
				for (auto e = root->child.begin(); e != root->child.end(); e++) {
					if (e->second == NULL) {
						continue;
					}
					evaluate_result[e->first] = (double)e->second->value / e->second->visit;
					evaluate_visit [e->first] = e->second->visit;
				}
			}

			Node* nd = root;

			// Select 
			while (nd->child.size() == nd->actionSet.size())
				nd = Select(nd, true);

			for (int i = 0; i < THREADNUM; i++) {
				thread_pool.addTask([nd, i]() {
					ExpandSimulate(nd, i);
				});
			}
			
			thread_pool.wait_all();

			// Backpropagate
			int visit = 0,
				value = 0, 
				player = -nd->state->player;

			for (auto e = nd->child.begin(); e != nd->child.end(); e++) {
				if (e->second != NULL) {
					visit++;
					value += e->second->value;
				}
			}

			while (nd != NULL) {
				nd->value += nd->state->player == player ? value : visit - value;
				nd->visit += visit;
				nd = nd->parent; 
			} 
		}
	}


	/*
	 *  禁入点、眼点标记, not allow suicide
	 */
	inline void judgeBanAndEye(Go::State& s, Go::Color player) {

		for (int i = 0; i < BOARD_COLOR_NUM; i++) {
			if (s.board[i] != 0) continue;

			char isEye = 0x7F;
			bool isBan = 1;

			for (int j = 0; j < 4; j++) {
				int v = Go::neighborhood(i, j);
				if (v == -1)
					continue;

				//核心判断
				if ((s.board[v] == 0)) {
					isEye = isBan = 0;
					break;
				}
				if (isEye == 0x7F)
					isEye = s.board[v];

				if (s.board[v] != isEye || s.qi[s.mark[v]] == 1)
					isEye = 0; 	//同一色，且均非一气

				if ((s.board[v] == player && s.qi[s.mark[v]] != 1) ||	//若是我，应只一气
					(s.board[v] != player && s.qi[s.mark[v]] == 1)) 	//若是敌，应必不只一气
					isBan = 0;
			}

			if (isEye != 0)
				s.mark[i] = EYEPOINT * isEye;
			if (isBan)
				s.mark[i] = BANPOINT * player;
		}
	}

	inline void ExpandSimulate(Node* nd, int id) {
		// Expand
		int n = nd->actionSet.size();

		for(int i = id; i < n; i += THREADNUM) {
			// new State
			Go::State 
				 * s_ = new Go::State(),
				 * s  = nd->state;
			*s_ = *s;
			s_->player = Go::Color(-s->player);
			s_->action = nd->actionSet[i];

			// new Node 
			Node* newNode = NULL;

			if (Go::updateState(*s_)) {
				judgeBanAndEye(*s_, Go::Color(-(*s_).player));

				// Simulate
				int reward = Simulate(*s_);

				newNode = new Node(s_, nd);
				newNode->visit += 1;
				newNode->value += s_->player == reward ? 1 : 0;
			} 

			mutex_.lock();
			nd->child[nd->actionSet[i]] = newNode;
			mutex_.unlock();
		}
	}

	/*
	 *  [1] Select
	 */
	inline Node* Select(Node* node, bool isExplore) {
		double maxValue = -DBL_MAX;
		Node* bestNode = NULL;

		for (auto e = node->child.begin(); e != node->child.end(); e++) {
			if (e->second == NULL)
				continue;
			
			// UCB
			double 
				a = (double)e->second->value / e->second->visit,
				b = (double)log(node->visit) / e->second->visit,
				t = a + (isExplore == true ? sqrt(2) : 0) * sqrt(b);

			maxValue = maxValue < t ? (bestNode = e->second, t) : maxValue;
		}
		return bestNode;
	} 

	/*
	 *  [3]  Simulate
	 */
	inline int Simulate(Go::State& _s) {
		Go::State s = _s;

		while (!Go::isTermination(s)) {
			s.player = Go::Color(-s.player);

			// count how many actions we can choose
			int num = 0;
			int randnum = rand();
			for (int i = 0; i < BOARD_COLOR_NUM; i++) {
				if (s.mark[i] == -1)
					num++;
			}

			// randomly choose an action
			while (num > 0) {
				Go::State s_ = s;
				int index = randnum % num + 1;

				for (int i = 0; i < BOARD_COLOR_NUM; i++) {
					if (s.mark[i] == -1)
						index--;

					if (index == 0) {
						s.action = i;
						break;
					}
				}

				if (Go::updateState(s)) {
					judgeBanAndEye(s, Go::Color(-s.player));
					break;
				}
				else {  
					// if this action is invalid
					num--;
					s_.mark[s.action] = BANPOINT;
					s = s_;
					s.player = Go::Color(-s.player);
				}
			} 

			if (num == 0) {
				s.action = PASS; 
				Go::updateState(s); 
				judgeBanAndEye(s, Go::Color(-s.player));
			}
		}
		int reward = Go::computeReward(s);
		return reward;
	}
	
	/*
	 *  move to a new root
	 */
	inline void move(Node*& root, int a) {
		// move to new root
		Node* t = root;
		root = root->child[a]; 
		root->parent = NULL;

		// delete old root and its all child nodes except the best node
		t->state = NULL;
		t->child[a] = NULL;
		delete t;

		return;
	}

}
#endif