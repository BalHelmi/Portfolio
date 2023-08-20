import random
import json

import torch
from model import NeuralNetwork
from nltk_utils import bag_of_words, tokenize, stem

device = torch.device('cpu')

with open("intents.json", 'r') as f:
    intents = json.load(f)

FILE = 'data.pth'
data = torch.load(FILE)

input_size = data['input_size']
output_size = data['output_size']
hidden_size = data['hidden_size']
tags = data['tags']
words = data['words']
model_state = data['model_state']

model = NeuralNetwork(input_size, hidden_size, output_size).to(device)
model.load_state_dict(model_state)
model.eval()

bot_name = "kiwi"

def get_response(msg):
    sentence = tokenize(msg)
    X = bag_of_words(sentence, words)
    X = X.reshape(1, X.shape[0])
    X = torch.from_numpy(X).to(device)
    
    output = model(X)
    _, predicted = torch.max(output, dim= 1)
    tag = tags[predicted.item()]
    
    probs = torch.softmax(output, dim=1)
    prob = probs[0][predicted.item()]
    if prob.item() > 0.75:
        for intent in intents['intents']:
            if tag == intent["tag"]:
                return random.choice(intent['responses'])
    else:
        return "I do not understand..."
