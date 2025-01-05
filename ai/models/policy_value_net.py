import torch
import torch.nn as nn
import torch.nn.functional as F

class PolicyValueNet(nn.Module):
    def __init__(self, board_size):
        super(PolicyValueNet, self).__init__()
        self.board_size = board_size
        self.conv1 = nn.Conv2d(17, 256, kernel_size=3, padding=1)
        self.res_blocks = nn.ModuleList([self._build_res_block() for _ in range(19)])
        self.policy_head = nn.Sequential(
            nn.Conv2d(256, 2, kernel_size=1),
            nn.Flatten(),
            nn.Linear(2 * board_size * board_size, board_size * board_size)
        )
        self.value_head = nn.Sequential(
            nn.Conv2d(256, 1, kernel_size=1),
            nn.Flatten(),
            nn.Linear(board_size * board_size, 256),
            nn.ReLU(),
            nn.Linear(256, 1),
            nn.Tanh()
        )

    def _build_res_block(self):
        return nn.Sequential(
            nn.Conv2d(256, 256, kernel_size=3, padding=1),
            nn.BatchNorm2d(256),
            nn.ReLU(),
            nn.Conv2d(256, 256, kernel_size=3, padding=1),
            nn.BatchNorm2d(256)
        )

    def forward(self, x):
        x = F.relu(self.conv1(x))
        for block in self.res_blocks:
            x = x + block(x)
        policy = self.policy_head(x)
        value = self.value_head(x)
        return policy, value
