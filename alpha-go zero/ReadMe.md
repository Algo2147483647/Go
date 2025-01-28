### 文件说明

1. **核心流程**：
   - 初始化游戏逻辑 (`GoGame`) 和神经网络 (`AlphaZeroNet`)。
   - 使用 `SelfPlay` 生成自我对弈数据。
   - 使用 `Trainer` 训练神经网络。
   - 根据用户输入的参数，支持加载和保存模型。

2. **参数化支持**：
   - 用户可以通过命令行参数调整棋盘大小、模拟次数、自我对弈局数、训练超参数等。

---

### 示例运行

1. 默认运行（9x9 棋盘，自我对弈 10 局）：
   ```bash
   python main.py
   ```

2. 自定义参数（19x19 棋盘，自我对弈 50 局，学习率调整为 0.0005）：
   ```bash
   python main.py --board_size 19 --self_play_games 50 --learning_rate 0.0005
   ```

3. 加载已有模型并训练（19x19 棋盘）：
   ```bash
   python main.py --board_size 19 --load_model saved_model.pth --save_model new_model.pth
   ```
