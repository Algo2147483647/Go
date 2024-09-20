#ifndef GO_STATE_H
#define GO_STATE_H

#include "config.h"
#include "zobristHash.h"

using namespace std;

namespace Go {
	class State {
	public:
		// action (x, y, c)
		short action = -2;
		Color player = WHITE;

		array<Color, BOARD_COLOR_NUM> board;
		array<short, BOARD_COLOR_NUM> mark, next;
		array<unsigned char, BOARD_COLOR_NUM> qi; 

		vector<uint64_t> historyState;

		State() {
			fill(board.begin(),  board.end(), EMPTY);
			fill(mark.begin(),   mark.end(), -1);
			fill(next.begin(),   next.end(), -1);
			fill(qi.begin(),     qi.end(),    0);

			historyState.push_back(0);
		}
	};

	/*
	 *  judge whether a state is terminated
	 */
	inline bool isTermination(State& s) {
		if (s.action == PASS &&
			s.historyState.size() >= 3 &&
			s.historyState[s.historyState.size() - 2] == s.historyState[s.historyState.size() - 3])
			return true;
		return false;
	}

	inline int neighborhood(int p, int i) {
		const static int
			adj_x[] = { 0, 0, 1,-1 },
			adj_y[] = { 1,-1, 0, 0 };

		int x = p % BOARD_SIZE + adj_x[i],
			y = p / BOARD_SIZE + adj_y[i],
			v = y * BOARD_SIZE + x;

		if (x < 0 || x >= BOARD_SIZE ||
			y < 0 || y >= BOARD_SIZE)
			return -1;
		return v;
	}

}
#endif
