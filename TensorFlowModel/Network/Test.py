from lib2to3.pytree import convert
from turtle import shape
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.datasets import mnist
from HexToCArray import *
import os
import pandas as pd

# (train_X, train_Y), (test_X, test_Y) = mnist.load_data()
# train_X = train_X/255.0 
# test_X = test_X/255.0
df = pd.read_csv('AllData.csv')
df = df.sample(frac=1)

test_size = int(len(df) * 0.1)
train = df.iloc[:-test_size,:].copy() 
test = df.iloc[-test_size:,:].copy()

train_Y = train['Activity']
train_X = train.drop(columns=('Activity'))

test_Y = test['Activity']
test_X = test.drop(columns=('Activity'))

print(test_X.shape)
print(test_Y.shape)
print(train_X.shape)
print(train_Y.shape)

model = keras.Sequential([
    layers.Input(shape=(test_X.shape[1])),
    layers.Dense(24, activation='relu'),
    layers.Dense(10, activation='relu'),
    layers.Dense(10, activation='relu'),
    layers.Dense(10, activation='relu'),
    layers.Dense(10, activation='relu'),
    layers.Dense(30, activation='relu'),
    layers.Dense(5, 'softmax')
])

model.summary()

model.compile(optimizer='adam',loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])

model.fit(train_X, train_Y, epochs = 20)

model.evaluate(test_X, test_Y)



# model.fit(
#     train_dataset,
#     steps_per_epoch=1000,
#     epochs=4
# )


# converter = tf.lite.TFLiteConverter.from_keras_model(model)
# converter.optimizations = [tf.lite.Optimize.DEFAULT]
# tflite_model = converter.convert()

# open("TfLiteModel.tflite", "wb").write(tflite_model)

# os.system('xxd -i TfLiteModel.tflite > MnistModel.h')



# model.fit(
#     train_dataset,
#     steps_per_epoch=1000,
#     epochs=4
# )


# converter = tf.lite.TFLiteConverter.from_keras_model(model)
# converter.optimizations = [tf.lite.Optimize.DEFAULT]
# tflite_model = converter.convert()

# open("TfLiteModel.tflite", "wb").write(tflite_model)

# os.system('xxd -i TfLiteModel.tflite > MnistModel.h')













