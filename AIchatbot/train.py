import json
from nltk_utils import tokenize, stem, bag_of_words
import numpy as np

import torch
import torch.nn as nn
from torch.utils.data import Dataset, DataLoader
from model import NeuralNetwork

with open('intents.json', 'r') as f:
    intents = json.load(f)
    
words = []
tags = []
data = []

for intent in intents['intents'] :
    tag = intent['tag']
    tags.append(tag)
    for pattern in intent['patterns'] :
        token = tokenize(pattern)
        words.extend(token)
        data.append((token, tag))
        
ignored_word = ['!', '?', '.']
words = [stem(w) for w in words if w not in ignored_word]
words = sorted(set(words))
tags = sorted(set(tags))


X_train = []
Y_train = []
for (pattern, tag) in data : 
    bag = bag_of_words(pattern, words)
    X_train.append(bag)
    
    label = tags.index(tag)
    Y_train.append(label)

X_train = np.array(X_train)
Y_train = np.array(Y_train)

class ChatDataset(Dataset):
    
    def __init__(self):
        self.nsamples = len(X_train)
        self.x_data = X_train
        self.y_data = Y_train
    
    def __getitem__(self, index):
        return self.x_data[index], torch.tensor(self.y_data[index], dtype=torch.long)
    
    def __len__(self):
        return self.nsamples

# Hyperparameters
batch_size = 8
input_size = len(X_train[0])
hidden_size = 8
output_size = len(tags)
learning_rate = 0.001
n_epochs = 2000

# Dataset
dataset = ChatDataset()
train_loader = DataLoader(dataset=dataset, batch_size=batch_size, shuffle=True, num_workers=0)

# Model
device = torch.device('cpu')
model = NeuralNetwork(input_size, hidden_size, output_size).to(device)
# Loss fuction and optimizer

loss_function = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr = learning_rate)

for epoch in range(n_epochs):
    for (wwords, labels) in train_loader:
        wwords = wwords.to(device)
        labels = labels.to(device)
        
        # Forward 
        outputs = model(wwords)
        loss = loss_function(outputs, labels)
        
        # Backward
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    if (epoch+1) % 100 == 0:
        print (f'Epoch [{epoch+1}/{n_epochs}], Loss: {loss.item():.4f}')

print(f'final loss, loss = {loss.item():.4f}') 

data = {
    "model_state": model.state_dict(),
    "input_size": input_size,
    "output_size": output_size,
    "hidden_size": hidden_size,
    "words": words,
    "tags": tags
}  

FILE = "data.pth"

torch.save(data, FILE)

print(f'training completed, File Saved to {FILE}')
    