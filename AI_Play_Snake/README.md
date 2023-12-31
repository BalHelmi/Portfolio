# AI play snake Project

## Description

This project is about to teach AI to play sanke using reinforcement learning with Pytorch and Pygame.

## Table of Contents

- [References](#references)
- [Rules](#Rules)
- [Report](#report)
- [Installation](#installation)
- [Usage](#usage)
- [Dependencies](#dependencies)

## References 

[PyTorch](https://pytorch.org/) : a popular framework for building and training neural networks.

[Pygame](https://pygame.org/) : a simple way to create interactive games with python.

## Rules 
1. The snake game involves a snake that moves around a board, eating food and avoiding obstacles. 
2. The goal of the game is to make the snake as long as possible by eating food without crashing into the walls or running into its own body, which will end the game. 
3. The snake grows in length each time it eats a piece of food.

## Report
https://docs.google.com/document/d/1LuE8GuE_lenhlUY1_Fsm9z8oaaizipsC7hFQ8JfUDY0/edit?usp=sharing

## Installation
Create a virtual environment with conda 

```
conda create -n pysnake_env python=3.10
```  

Install all dependencies
```
conda activate pysnake_env
pip install -r requirements.txt
```  

## Usage
```
python agent.py
```  
## Dependencies
pygame
torch
torchvision
matplotlib
ipython
numpy