import argparse
from alphazero.self_play import SelfPlay
from alphazero.training import Trainer
from alphazero.neural_net import AlphaZeroNet
from games.go import GoGame

def main(args):
    # 初始化游戏和神经网络
    print("初始化游戏和神经网络...")
    game = GoGame(board_size=args.board_size)
    neural_net = AlphaZeroNet(game.board_size)

    # 加载已有模型（如果指定路径）
    if args.load_model:
        print(f"加载模型：{args.load_model}")
        neural_net.load_model(args.load_model)

    # 自我对弈模块
    print("开始自我对弈以生成训练数据...")
    self_play = SelfPlay(game, neural_net, simulations=args.simulations, temperature=args.temperature)
    training_data = self_play.generate_data(games=args.self_play_games)

    # 训练模块
    print("开始训练神经网络...")
    trainer = Trainer(neural_net, lr=args.learning_rate, batch_size=args.batch_size, epochs=args.epochs)
    trainer.train(training_data)

    # 保存模型
    if args.save_model:
        print(f"保存模型到：{args.save_model}")
        trainer.save_model(args.save_model)

    print("AlphaZero 流程完成！")

if __name__ == "__main__":
    # 定义命令行参数
    parser = argparse.ArgumentParser(description="运行 AlphaZero 主流程")
    parser.add_argument("--board_size", type=int, default=9, help="棋盘大小（默认9x9）")
    parser.add_argument("--self_play_games", type=int, default=10, help="自我对弈的局数")
    parser.add_argument("--simulations", type=int, default=800, help="每次 MCTS 的模拟次数")
    parser.add_argument("--temperature", type=float, default=1.0, help="MCTS 温度参数")
    parser.add_argument("--learning_rate", type=float, default=0.001, help="训练的学习率")
    parser.add_argument("--batch_size", type=int, default=64, help="训练的批量大小")
    parser.add_argument("--epochs", type=int, default=10, help="训练的轮数")
    parser.add_argument("--load_model", type=str, help="加载已有模型的路径")
    parser.add_argument("--save_model", type=str, default="alphazero_model.pth", help="保存模型的路径")
    args = parser.parse_args()

    # 运行主流程
    main(args)
