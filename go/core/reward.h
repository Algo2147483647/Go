#ifndef GO_REWARD_H
#define GO_REWARD_H

#include "config.h"
#include "state.h"

using namespace std;

namespace Go {
	/*
	 *	calculate reach color to help compute reward
	 */
	inline int calculateReachColor(array<Color, BOARD_COLOR_NUM>& board, const int color) {
		auto reachable = 0;
		auto bd = vector<bool>(BOARD_COLOR_NUM, false);
		auto open = queue<int>();

		for (auto i = 0; i < BOARD_COLOR_NUM; i++) {
			if (board[i] == color) {
				reachable++;
				bd[i] = true;
				open.push(i);
			}
		}

		while (!open.empty()) {
			auto v = open.front();
			open.pop();

			for (auto j = 0; j < 4; j++) {
				int vt = neighborhood(v, j);

				if (vt != -1 && !bd[vt] && board[vt] == EMPTY) {
					reachable++;
					bd[vt] = true;
					open.push(vt);
				}
			}
		}
		return reachable;
	}

	/*
	 *  compute reward of termination state and judge victory
	 */
	inline Color computeReward(State& s) {
		auto white = calculateReachColor(s.board, WHITE);
		auto black = calculateReachColor(s.board, BLACK);
		return (black - white - KOMI) >= 0 ? BLACK : WHITE;
	}
}
#endif
