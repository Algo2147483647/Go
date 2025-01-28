import numpy as np
from mcts.mcts import MCTSNode


class SelfPlay:
    """自我对弈模块，用于生成训练数据。"""

    def __init__(self, game, neural_net, simulations=800, temperature=1.0):
        """
        初始化自我对弈模块。

        Args:
            game: 游戏逻辑对象。
            neural_net: 神经网络对象，用于指导 MCTS。
            simulations: 每次搜索的模拟次数。
            temperature: 温度参数，控制探索和利用。
        """
        self.game = game
        self.neural_net = neural_net
        self.simulations = simulations
        self.temperature = temperature

    def play_game(self):
        """进行一局自我对弈，生成训练数据。

        Returns:
            A list of tuples (state, pi, reward), 其中：
                - state: 游戏状态。
                - pi: 动作分布（由 MCTS 提供）。
                - reward: 最终的胜负结果。
        """
        game_states = []
        mcts = MCTS(self.game, self.neural_net, self.simulations)

        # 开始游戏
        state, player = self.game.get_initial_state()
        while not self.game.is_terminal():
            # 获取动作概率分布 pi
            action_probs = mcts.get_action_probs(state, self.temperature)
            game_states.append((state.copy(), action_probs, player))

            # 执行动作
            action = np.random.choice(len(action_probs), p=action_probs)
            state, player = self.game.perform_action(self._index_to_action(action))

        # 计算奖励
        reward = self.game.reward()
        training_data = [(s, pi, reward if p == player else -reward) for s, pi, p in game_states]
        return training_data

    def generate_data(self, games=100):
        """生成指定局数的训练数据。

        Args:
            games: 需要生成的游戏局数。

        Returns:
            A list of训练数据，用于后续训练。
        """
        all_data = []
        for game_index in range(games):
            print(f"开始自我对弈：第 {game_index + 1} 局")
            data = self.play_game()
            all_data.extend(data)
        return all_data

    def _index_to_action(self, index):
        """将索引转换为游戏中的实际动作。"""
        board_size = self.game.board_size
        return index // board_size, index % board_size