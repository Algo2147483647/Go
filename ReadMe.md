# Go & AI

[TOC]

## Define

[Go](./Go.md)

## Core

|file name|function|
|---|---|
|[Go.h](Go.h)| main |
|[include.h](include.h)| include |
|[state.h](state.h)| state |
|[action.h](action.h)| action |
|[reward.h](reward.h)| reward |
|[updateState.h](updateState.h)| update state |
|||

### State

```cpp
class State {
    public:
    short action = -2;
    Color player = BLACK;

    array<Color, BOARD_COLOR_NUM> board;
    array<short, BOARD_COLOR_NUM> mark, next;
    array<unsigned char, BOARD_COLOR_NUM> qi;

    vector<uint64_t> historyState;
};
```

- action：动作
  - PASS 跳过该局
  - 棋盘中的位置

- player：玩家颜色
- board：棋盘
- mark：标记一个颜色所属的连通域的序号, 序号也是闭合的环路的头位置
- next: 同一颜色连通域的颜色，通过next 后继构成闭合的环路，方便遍历该连通域。
- qi：记录连通域序号到气数的映射

#### Termination

- 2 $\times$ PASS

#### Zobrist Hash

```cpp
static array<array<uint64_t, BOARD_SIZE * BOARD_SIZE>, 3> zobristTable;

inline uint64_t randuint64() {
std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<uint64_t> dis;
return dis(gen);
}

inline void zobrist_init() {
for (int i = 0; i < 3; i++) {
for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
zobristTable[j][i] = randuint64();
}
}
};

inline uint64_t& zobrist_hash(uint64_t& hashValue, int p, Color color) {
hashValue ^= zobristTable[p][color + 1];
return hashValue;
}
```

### Action

#### Pass

跳过该局

#### Disable global isomorphism

检查当前传入的状态 state 的棋盘的zobrist hash 值，是否在历史上同一棋手处存在过。

#### Not allow suicide

### $S_t \overset{A_t}{\rightarrow} S_{t+1}$

<img src="assets/Update_State.svg" alt="assets/Update_State.svg" style="zoom:50%;" />

#### Remove stone block with no Qi

输入要删除的连通块的序号，遍历块的颜色将其改为空点，并且将周围的块的气加一。

#### Merge stone block

输入两个连通块的序号，也是头位置，遍历块1的颜色都改为块2序号，并看周围空点是否为块1、2的共用空点，如果不是，块2的气加一. 最后交换两个头的后继，合并为1个连通块。

### Reward

<img src="assets/Terminate.svg" alt="Terminate" style="zoom:50%;" />

#### Score: Number of spaces reachable

1. 函数`calculateReachColor`接收两个参数：一个颜色数组`board`和一个表示特定颜色的整数`color`。`BOARD_COLOR_NUM`是一个预定义的常量，表示`board`数组的大小。
2. `reachable`变量初始化为0，它用于记录可达的空格数量。
3. `bd`是一个布尔类型的向量，初始化所有元素为`false`，用于标记网格中的每个位置是否已经被访问过。
4. `open`是一个队列，用于进行宽度优先搜索（BFS）。
5. 第一个`for`循环遍历游戏板上的所有位置。如果某个位置的颜色与给定的颜色相同，这个位置就被认为是可达的，`reachable`会增加，相应的`bd`数组位置会被标记为`true`，并且该位置会被加入到`open`队列中。
6. 接下来的`while`循环是一个宽度优先搜索，用于找到所有从起始颜色出发可达的`EMPTY`（未被占据的）位置。这个循环会一直进行，直到没有更多的位置需要检查。
7. 在循环内部，它取出队列`open`的前端元素，代表当前正在检查的位置。
8. 接着，内部的`for`循环遍历该位置的四个相邻位置（由`adj_x`和`adj_y`数组给出的偏移量表示）。对于每个相邻位置，计算新的`xt`和`yt`坐标，以及`vt`，即该位置在`board`数组中的索引。
9. 如果计算出的相邻位置坐标在棋盘内，并且该位置未被访问过，同时位置颜色为`EMPTY`，则将其标记为已访问，并将该位置的索引加入到`open`队列中。
10. 最后，函数返回`reachable`的值，即从给定颜色出发，可以到达的`EMPTY`位置的总数。

#### Win or loss calculation (Komi)

		float komi = 7.5;
	
		auto white = calculateReachColor(s.board, WHITE);
		auto black = calculateReachColor(s.board, BLACK);
		return (black - white - komi) >= 0 ? BLACK : WHITE;

## AI 

|file name|function|
|---|---|
|[Go_AI_Multithread.h](Go_AI_Multithread.h)| AI of Go |
|[Go_AI.h](Go_AI.h)| old AI |
|||

## Demo

|file name|function|
|---|---|
|||

### Operate

- Press mouse left button: move stone
- Press mouse middle button: return to the previous state
- Press key P: Pass
- Press key A: open AI
- Press key S: display AI evaluation results
- Press key Q: display qi of each stone
- Press key N: display order of each stone
- Press key M: display mark and ID of each stone