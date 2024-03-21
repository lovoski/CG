import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data.dataset import Dataset
from torch.utils.data.dataloader import DataLoader

# Network definitions

class Gating(nn.Module):
  def __init__(self, input_size, hidden_size, output_size):
    super(Gating, self).__init__()
    self.activation = nn.ELU()
    self.first_layer = nn.Linear(input_size, hidden_size)
    self.second_layer = nn.Linear(hidden_size, hidden_size)
    self.third_layer = nn.Linear(hidden_size, output_size)

  def forward(self, X):
    mid1 = self.activation(self.first_layer(X))
    mid2 = self.activation(self.second_layer(mid1))
    return self.third_layer(mid2)

class MotionPrediction(nn.Module):
  def __init__(self, input_size, hidden_size, output_size, experts_num):
    super(MotionPrediction, self).__init__()
    self.activation = nn.ELU()
    self.input_size = input_size
    self.hidden_size = hidden_size
    self.output_size = output_size
    self.experts_num = experts_num
    self.first_layer = []
    self.second_layer = []
    self.third_layer = []
    for i in range(experts_num):
      self.first_layer.append(nn.Linear(input_size, hidden_size))
      self.second_layer.append(nn.Linear(hidden_size, hidden_size))
      self.third_layer.append(nn.Linear(hidden_size, output_size))
    # Use xavier uniform initialization
    for i in range(experts_num):
      nn.init.xavier_uniform_(self.first_layer[i].weight)
      nn.init.zeros_(self.first_layer[i].bias)
      nn.init.xavier_uniform_(self.second_layer[i].weight)
      nn.init.zeros_(self.second_layer[i].bias)
      nn.init.xavier_uniform_(self.third_layer[i].weight)
      nn.init.zeros_(self.third_layer[i].bias)

  def forward(self, X : torch.Tensor, W : torch.Tensor):
    # Apply softmax to smooth factors
    F.softmax(W, dim=1)
    # Blend the results of each layer
    mid1 = torch.zeros((X.shape[0], self.hidden_size))
    for i in range(self.experts_num):
      mid1 += W[:, i].reshape((X.shape[0], 1)).repeat((1, self.hidden_size)) * self.first_layer[i](X)
    mid1 = self.activation(mid1)
    mid2 = torch.zeros((X.shape[0], self.hidden_size))
    for i in range(self.experts_num):
      mid2 += W[:, i].reshape((X.shape[0], 1)).repeat((1, self.hidden_size)) * self.second_layer[i](mid1)
    mid2 = self.activation(mid2)
    mid3 = torch.zeros((X.shape[0], self.output_size))
    for i in range(self.experts_num):
      mid3 += W[:, i].reshape((X.shape[0], 1)).repeat((1, self.output_size)) * self.third_layer[i](mid2)
    return mid3

class Model(nn.Module):
  def __init__(self, input_size, hidden_size, output_size, experts_num, lr, epochs):
    super(Model, self).__init__()
    self.gating = Gating(input_size, hidden_size, experts_num)
    self.motion_prediction = MotionPrediction(input_size, hidden_size, output_size, experts_num)
    self.input_size = input_size
    self.hidden_size = hidden_size
    self.output_size = output_size
    parameters = []
    # Manually assign the parameters to be trained
    for i in range(experts_num):
      # Expert weights
      parameters.append({'params':self.motion_prediction.first_layer[i].parameters()})
      parameters.append({'params':self.motion_prediction.second_layer[i].parameters()})
      parameters.append({'params':self.motion_prediction.third_layer[i].parameters()})
    # Gating weight
    parameters.append({'params':self.gating.parameters()})
    self.trainer = torch.optim.AdamW(parameters, lr)
    self.loss = nn.MSELoss()
    self.epochs = epochs

  def train(self, X, Y):
    for epoch in range(self.epochs):
      self.trainer.zero_grad()
      W = self.gating(X)
      pred = self.motion_prediction(X, W)
      l = self.loss(pred, Y)
      l.backward()
      self.trainer.step()
      if epoch % 100 == 0:
        print(f'epoch={epoch}, loss={l.item()}, pred={pred}')

  def predict(self, X):
    W = self.gating(X)
    return self.motion_prediction(X, W)

# Inputs and Outputs

X = torch.tensor([[1, 2, 3, 4, 5], [2, 3, 4, 5, 6]], dtype=torch.float32)
Y = torch.tensor([[6, 7, 8, 9, 10], [7, 8, 9, 10, 11]], dtype=torch.float32)

# Training

model = Model(5, 20, 5, 4, 1e-2, 1000)
model.train(X, Y)
