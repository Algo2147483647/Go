#ifndef GO_UPDATE_STATE_H
#define GO_UPDATE_STATE_H

#include "config.h"
#include "action.h"

using namespace std;

namespace Go {
	/*
	 *  remove stone block
	 */
	inline void removeBlock(State& s, int p) {
		int p_ = p;
		int buf[4], bufcur = 0;

		do {
			zobrist_hash(s.historyState.back(), p_, s.board[p_]);
			s.board[p_] = EMPTY;
			s.mark [p_] = -1;

			bufcur = 0;

			for (int j = 0; j < 4; j++) {
				int v = neighborhood(p_, j);

				if (v != -1 && s.board[v] != EMPTY)
					buf[bufcur++] = s.mark[v];
			}

			if (bufcur != 0) {
				sort(buf, buf + bufcur);

				for (int j = 0; j < bufcur; j++) {
					if (j == 0)
						s.qi[buf[j]] ++;
					else if (buf[j] != buf[j - 1])
						s.qi[buf[j]] ++;
				}
			}
		} while ((p_ = s.next[p_]) != p);

		return;
	}

	/*
	 *  merge stone block
	 */
	inline void mergeBlock(State& s, int p1, int p2) {
		if (p1 < p2)
			swap(p1, p2);

		int p = p1;

		do {
			for (int j = 0; j < 4; j++) {
				int v = neighborhood(p, j),
					found = 0;

				if (v == -1 || s.board[v] != EMPTY)
					continue;

				for (int k = 0; k < 4; k++) {
					int v_ = neighborhood(v, k);

					if (v_!= -1 && s.mark[v_] == p2) {
						found = 1;
						break;
					}			
				}
				if (!found)
					s.qi[p2] ++;
			}
			// update mark and avoid repeated search 
			s.mark[p] = p2; 
		} while ((p = s.next[p]) != p1);

		swap(s.next[p1], s.next[p2]);

		return;
	}

	/*
	 *  update state with move stone
	 */
	inline bool updateState(State& s) {
		// Pass
		if (s.action == PASS) {
			s.historyState.push_back(s.historyState.back());
			return true;
		}

		// not a legal move
		if (s.board[s.action] != EMPTY || isSuicide(s))
			return false;

		// play move
		s.board[s.action] = s.player;
		s.mark [s.action] = s.action;
		s.next [s.action] = s.action;
		s.qi   [s.action] = 0;
		s.historyState.push_back(s.historyState.back());
		zobrist_hash(s.historyState.back(), s.action, s.player);

		for (int j = 0; j < 4; j++) {
			int v = neighborhood(s.action, j);

			if (v != -1 && s.board[v] == EMPTY)
				s.qi[s.action] ++;
		}

		// detect neighborhood
		int buf[4], bufcur = 0;

		for (int j = 0; j < 4; j++) {
			int v = neighborhood(s.action, j);

			if (v == -1 || s.board[v] == EMPTY)
				continue;

			buf[bufcur++] = s.mark[v];
		}

		// clear the opponent colors have no qi and merge stone blocks
		if (bufcur != 0) {
			sort(buf, buf + bufcur);

			for (int j = 0; j < bufcur; j++) {
				if (j == 0 || buf[j] != buf[j - 1]) {
					s.qi[buf[j]] --;

					if (s.board[buf[j]] == -s.player && s.qi[buf[j]] == 0) 
						removeBlock(s, buf[j]);
					else if(s.board[buf[j]] == s.player)
						mergeBlock(s, s.mark[s.action], buf[j]);
				}
			}
		}

		// disable global isomorphism
		if (judgeKo(s))
			return false;

		return true;
	}


}
#endif
