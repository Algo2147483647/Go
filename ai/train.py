import torch
import torch.optim as optim
from models.policy_value_net import PolicyValueNet

def train(data_loader, model, epochs, lr):
    optimizer = optim.Adam(model.parameters(), lr=lr)
    for epoch in range(epochs):
        for states, policies, values in data_loader:
            optimizer.zero_grad()
            pred_policies, pred_values = model(states)
            loss = compute_loss(pred_policies, pred_values, policies, values)
            loss.backward()
            optimizer.step()
