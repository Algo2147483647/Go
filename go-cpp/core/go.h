#ifndef GO_H
#define GO_H

#include "state.h"
#include "updateState.h"
#include "reward.h"
#include "zobristHash.h"

using namespace std;

namespace Go {
	/*
	 *  generate next state
	 */
	inline void go_init() {
		zobrist_init();
	}

	inline State& play(State& s, int action) {
		State s_ = s;
		s.player = (s.player == BLACK ? WHITE : BLACK);
		s.action = action;

		if(updateState(s))
			return s;
		return s = s_;
	}
}

#endif