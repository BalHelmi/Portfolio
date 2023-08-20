import nltk 
import numpy as np
#nltk.download('punkt')

from nltk.stem.porter import PorterStemmer

stemmer = PorterStemmer()
def tokenize(sentence):
    return nltk.word_tokenize(sentence)

def stem(word):
    return stemmer.stem(word.lower())

def bag_of_words(sentence, words):
    sentence = [stem(word) for word in sentence]
    bag = np.zeros(len(words), dtype=np.float32)
    for index, word in enumerate(words) : 
        if word in sentence:
            bag[index] = 1
    
    return bag


        