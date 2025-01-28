import torch
import torch.optim as optim
from models.policy_value_net import PolicyValueNet


class AlphaZeroDataset(Dataset):
    """自定义数据集，用于训练 AlphaZero 神经网络。"""

    def __init__(self, data):
        """
        Args:
            data: 列表 [(state, pi, reward)]。
        """
        self.data = data

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        state, pi, reward = self.data[idx]
        return torch.tensor(state, dtype=torch.float32), \
               torch.tensor(pi, dtype=torch.float32), \
               torch.tensor(reward, dtype=torch.float32)

class Trainer:
    """训练模块，用于优化神经网络。"""

    def __init__(self, neural_net, lr=0.001, batch_size=64, epochs=10):
        """
        初始化训练模块。

        Args:
            neural_net: 神经网络对象。
            lr: 学习率。
            batch_size: 每次训练的批量大小。
            epochs: 每次训练的轮数。
        """
        self.neural_net = neural_net
        self.lr = lr
        self.batch_size = batch_size
        self.epochs = epochs
        self.optimizer = torch.optim.Adam(self.neural_net.parameters(), lr=lr)
        self.loss_fn_policy = torch.nn.CrossEntropyLoss()  # 策略头损失
        self.loss_fn_value = torch.nn.MSELoss()  # 值头损失

    def train(self, training_data):
        """训练神经网络。

        Args:
            training_data: 训练数据 [(state, pi, reward)]。
        """
        dataset = AlphaZeroDataset(training_data)
        dataloader = DataLoader(dataset, batch_size=self.batch_size, shuffle=True)

        for epoch in range(self.epochs):
            total_policy_loss = 0
            total_value_loss = 0

            for state, pi, reward in dataloader:
                # 前向传播
                pred_pi, pred_value = self.neural_net(state)

                # 计算损失
                loss_policy = self.loss_fn_policy(pred_pi, pi)
                loss_value = self.loss_fn_value(pred_value.squeeze(-1), reward)

                # 后向传播与优化
                total_loss = loss_policy + loss_value
                self.optimizer.zero_grad()
                total_loss.backward()
                self.optimizer.step()

                total_policy_loss += loss_policy.item()
                total_value_loss += loss_value.item()

            print(f"Epoch {epoch + 1}/{self.epochs} | "
                  f"Policy Loss: {total_policy_loss:.4f} | Value Loss: {total_value_loss:.4f}")

        print("训练完成！")

    def save_model(self, path):
        """保存模型到指定路径。"""
        save_model(self.neural_net, path)

    def load_model(self, path):
        """从指定路径加载模型。"""
        load_model(self.neural_net, path)