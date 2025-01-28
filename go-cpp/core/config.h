#ifndef GO_INCLUDE_H
#define GO_INCLUDE_H

#include <stdio.h>
#include <vector>
#include <array>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;

namespace Go {
	static constexpr auto BOARD_SIZE = 19;
	static constexpr auto BOARD_COLOR_NUM = BOARD_SIZE * BOARD_SIZE;
	static constexpr auto PASS = -1;
	static constexpr auto KOMI = 7.5f;

	/*
	 * possible contents of a vertex
	 */
	enum Color : char {
		EMPTY = 0, BLACK = 1, WHITE = -1
	};
}
#endif