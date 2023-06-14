from lib2to3.pytree import convert
from turtle import shape
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.datasets import mnist
import os
import pandas as pd

from matplotlib import pyplot as plt

# (train_X, train_Y), (test_X, test_Y) = mnist.load_data()
# train_X = train_X/255.0 
# test_X = test_X/255.0
df = pd.read_csv('AllData.csv')
df = df.sample(frac=1)

df['Activity'] = df['Activity'].replace(3,2)

# df = df.drop(columns=('G'))

test_size = int(len(df) * 0.1)
train = df.iloc[:-test_size,:].copy() 
test = df.iloc[-test_size:,:].copy()

train_y = train['Activity']
train_x = train.drop(columns=('Activity'))

test_y = test['Activity']
test_x = test.drop(columns=('Activity'))

print(train_y.value_counts())

print(train_x.columns)
print(test_x.shape)
print(test_y.shape)
print(train_x.shape)
print(train_y.shape)



model = keras.Sequential([
    layers.Input(shape=(test_x.shape[1])),
    layers.Dense(24, activation='ReLU'),
    layers.Dropout(0.1),
    layers.Dense(24, activation='ReLU'),
    layers.Dropout(0.1),
    layers.Dense(24, activation='ReLU'),
    layers.Dropout(0.1),
    layers.Dense(24, activation='ReLU'),
    layers.Dropout(0.1),
    layers.Dense(3, 'softmax')
    ])

model.summary()

model.compile(optimizer='adam',loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])
model.fit(train_x, train_y, epochs = 50, batch_size=32, validation_data=(test_x,test_y))

acc_test = model.history.history

acc = model.evaluate(test_x, test_y)

plt.plot(acc_test['val_accuracy'])
plt.plot(acc_test['accuracy'])
plt.title('Skuteczność uczenia na przestrzeni epok')
plt.ylabel('Skuteczność')
plt.xlabel('Epoka')
plt.legend([ 'walidacyjne','treningowe'], loc='lower right', prop={'size': 15})
plt.show()

print(acc)


converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

open("TfLiteModel.tflite", "wb").write(tflite_model)

os.system('xxd -i TfLiteModel.tflite > MnistModel.h')














