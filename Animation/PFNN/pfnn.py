import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data.dataset import Dataset
from torch.utils.data.dataloader import DataLoader

# Network definition

class MotionPrediction(nn.Module):
  def __init__(self, input_size, hidden_size, output_size):
    super(MotionPrediction, self).__init__()
    self.input_size = input_size
    self.hidden_size = hidden_size
    self.output_size = output_size
    self.activation = nn.ReLU()
    self.L1 = []
    self.L2 = []
    self.L3 = []
    for i in range(4):
      self.L1.append(nn.Linear(input_size, hidden_size))
      self.L2.append(nn.Linear(hidden_size, hidden_size))
      self.L3.append(nn.Linear(hidden_size, output_size))
    # Initialize the model
    for i in range(4):
      nn.init.xavier_uniform_(self.L1[i].weight)
      nn.init.zeros_(self.L1[i].bias)
      nn.init.xavier_uniform_(self.L2[i].weight)
      nn.init.zeros_(self.L2[i].bias)
      nn.init.xavier_uniform_(self.L3[i].weight)
      nn.init.zeros_(self.L3[i].bias)

  def forward(self, X : torch.Tensor, phi : torch.Tensor):
    # Process the batched phase
    w = 2 / torch.pi * phi
    wi = w.to(torch.int32) % 4
    k = [(wi-1)%4, wi%4, (wi+1)%4, (wi+2)%4]
    coeff = [
      -0.5*w+w*w-0.5*w*w*w,
      -2.5*w*w+1.5*w*w*w,
      0.5*w+2*w*w-1.5*w*w*w,
      -0.5*w*w+0.5*w*w*w
    ]
    # MLP computation
    # Terrible performance, this should be optimized latter
    H1 = torch.zeros((X.shape[0], self.hidden_size))
    for i in range(4):
      H1 += coeff[i].reshape((X.shape[0], 1)).repeat((1, self.hidden_size)) * torch.stack([self.L1[j](X[ind, :]) for ind, j in enumerate(k[i])])
    H1 = self.activation(H1)
    H2 = torch.zeros((X.shape[0], self.hidden_size))
    for i in range(4):
      H2 += coeff[i].reshape((X.shape[0], 1)).repeat((1, self.hidden_size)) * torch.stack([self.L2[j](H1[ind, :]) for ind, j in enumerate(k[i])])
    H2 = self.activation(H2)
    H3 = torch.zeros((X.shape[0], self.output_size))
    for i in range(4):
      H3 += coeff[i].reshape((X.shape[0], 1)).repeat((1, self.output_size)) * torch.stack([self.L3[j](H2[ind, :]) for ind, j in enumerate(k[i])])
    return H3

class Model:
  def __init__(self, input_size, hidden_size, output_size, lr, epochs):
    self.motion_prediction = MotionPrediction(input_size, hidden_size, output_size)
    parameters = []
    for i in range(4):
      parameters.append({'params':self.motion_prediction.L1[i].parameters()})
      parameters.append({'params':self.motion_prediction.L2[i].parameters()})
      parameters.append({'params':self.motion_prediction.L3[i].parameters()})
    self.trainer = torch.optim.Adam(parameters, lr)
    self.loss = nn.MSELoss()
    self.epochs = epochs

  def train(self, X, p, Y):
    for epoch in range(self.epochs):
      self.trainer.zero_grad()
      pred = self.motion_prediction(X, p)
      l = self.loss(pred, Y)
      l.backward()
      self.trainer.step()
      if (epoch+1) % 100 == 0:
        print(f'epoch={epoch+1}, loss={l.item()}, pred={pred}')

# Inputs and Outputs

X = torch.tensor([
  [1, 2, 3],
  [4, 5, 6]
], dtype=torch.float32)
Y = torch.tensor([
  [2, 3, 4],
  [5, 6, 7]
], dtype=torch.float32)
phi = torch.tensor([
  0.2,
  1.3
], dtype=torch.float32)

model = Model(3, 10, 3, 1e-2, 1000)
model.train(X, phi, Y)
