import numpy as np

class Go:
    """围棋游戏逻辑实现。"""

    def __init__(self, board_size=19):
        self.board_size = board_size
        self.empty = 0
        self.black = 1
        self.white = -1
        self.reset()

    def reset(self):
        """初始化棋盘和游戏状态。"""
        self.board = np.zeros((self.board_size, self.board_size), dtype=int)
        self.current_player = self.black  # 黑棋先手
        self.history = []  # 保存棋盘历史，用于判断打劫等情况

    def get_initial_state(self):
        """返回初始状态。"""
        return self.board.copy(), self.current_player

    def is_legal_action(self, action):
        """判断某个动作是否合法。"""
        x, y = action
        if not (0 <= x < self.board_size and 0 <= y < self.board_size):
            return False  # 棋子位置在棋盘外
        if self.board[x, y] != self.empty:
            return False  # 棋子位置已被占用
        # 检查气（周围空格）
        temp_board = self.board.copy()
        temp_board[x, y] = self.current_player
        if self._has_liberty(temp_board, x, y):
            return True
        # 检查是否会导致自杀
        return False

    def get_legal_actions(self):
        """返回当前棋盘状态下所有合法动作。"""
        actions = []
        for x in range(self.board_size):
            for y in range(self.board_size):
                if self.is_legal_action((x, y)):
                    actions.append((x, y))
        return actions

    def perform_action(self, action):
        """执行动作并更新棋盘状态。"""
        if action is None:  # 跳过回合
            self.current_player *= -1
            return self.board, self.current_player

        x, y = action
        if not self.is_legal_action((x, y)):
            raise ValueError(f"非法动作：{action}")

        self.board[x, y] = self.current_player
        self._capture_stones(x, y)
        self.current_player *= -1  # 切换玩家
        self.history.append(self.board.copy())
        return self.board, self.current_player

    def is_terminal(self):
        """判断游戏是否结束（根据简单规则）。"""
        # 假设两个玩家连续跳过回合，游戏结束
        if len(self.history) >= 2 and self.history[-1] is None and self.history[-2] is None:
            return True
        return False

    def reward(self):
        """返回当前棋盘状态的奖励值（黑棋为正，白棋为负）。"""
        black_score = np.sum(self.board == self.black)
        white_score = np.sum(self.board == self.white)
        return black_score - white_score

    def _has_liberty(self, board, x, y):
        """检查某个棋子或棋子组是否有气（周围空格）。"""
        visited = set()

        def dfs(i, j):
            if (i, j) in visited:
                return False
            if not (0 <= i < self.board_size and 0 <= j < self.board_size):
                return False
            visited.add((i, j))
            if board[i, j] == self.empty:
                return True  # 有气
            if board[i, j] != board[x, y]:
                return False  # 遇到敌方棋子，终止
            # 深度优先搜索四个方向
            return any(dfs(i + di, j + dj) for di, dj in [(-1, 0), (1, 0), (0, -1), (0, 1)])

        return dfs(x, y)

    def _capture_stones(self, x, y):
        """清除被围死的对方棋子（无气的棋子）。"""
        opponent = -self.current_player
        captured = []

        def dfs(i, j):
            if not (0 <= i < self.board_size and 0 <= j < self.board_size):
                return
            if (i, j) in captured or self.board[i, j] != opponent:
                return
            captured.append((i, j))
            # 深度优先搜索四个方向
            for di, dj in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                dfs(i + di, j + dj)

        for di, dj in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            nx, ny = x + di, y + dj
            if 0 <= nx < self.board_size and 0 <= ny < self.board_size and self.board[nx, ny] == opponent:
                captured.clear()
                dfs(nx, ny)
                if not any(self._has_liberty(self.board, cx, cy) for cx, cy in captured):
                    for cx, cy in captured:
                        self.board[cx, cy] = self.empty
