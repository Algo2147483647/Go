import numpy as np
import torch

def encode_board(board, board_size):
    """
    将棋盘状态编码成神经网络输入的张量
    :param board: 当前棋盘的状态，二维 numpy 数组（0表示空白，1表示黑子，-1表示白子）
    :param board_size: 棋盘的尺寸（通常是 19x19）
    :return: 编码后的张量（形状：(17, board_size, board_size)）
    """
    # 17个通道，其中包括当前玩家的棋子、上一步的棋子等
    encoded_board = np.zeros((17, board_size, board_size), dtype=np.float32)
    
    # 当前玩家的棋子（假设1为黑子，-1为白子）
    encoded_board[0] = (board == 1).astype(np.float32)  # 黑子
    encoded_board[1] = (board == -1).astype(np.float32)  # 白子
    
    # 增加更多的特征层，比如合法行动图，最近的棋步等
    # encoded_board[2:] = ... 
    
    return encoded_board



def evaluate_model(model, board_input):
    """
    使用策略价值网络评估棋盘输入
    :param model: 策略价值网络模型
    :param board_input: 编码后的棋盘输入
    :return: 策略概率和价值
    """
    with torch.no_grad():
        board_input = torch.tensor(board_input).unsqueeze(0)  # 增加batch维度
        policy, value = model(board_input)
        return policy.squeeze(0).numpy(), value.squeeze(0).item()
