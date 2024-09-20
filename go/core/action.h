#ifndef GO_ACTION_H
#define GO_ACTION_H

#include "config.h"
#include "state.h"

using namespace std;

namespace Go {
	/*
	 *  Disable global isomorphism
	 */
	inline bool judgeKo(State& state) {
		for (int i = state.historyState.size() - 3; i >= 0; i -= 2)
			if (state.historyState.back() == state.historyState[i]) 
				return true;
		return false;
	}

	/*
	 *  not allow suicide
	 */
	inline bool isSuicide(State& s) {
		for (int j = 0; j < 4; j++) {
			int v = neighborhood(s.action, j);
			if (v == -1) continue;

			if ( s.board[v] == EMPTY ||
				(s.board[v] == s.player && s.qi[s.mark[v]] != 1) || 	//若是我，应只一气
				(s.board[v] != s.player && s.qi[s.mark[v]] == 1)) 		//若是敌，应必不只一气
				return false;
		}
		return true;
	}
}
#endif
