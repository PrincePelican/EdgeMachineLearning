from lib2to3.pytree import convert
from turtle import shape
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.datasets import mnist
from HexToCArray import *
import os

# (train_X, train_Y), (test_X, test_Y) = mnist.load_data()
# train_X = train_X/255.0 
# test_X = test_X/255.0

# train_X = train_X.reshape(60000, 784)
# test_X = test_X.reshape(10000, 784)

def data_generator():
    while(True):
        number1 = np.random.uniform();
        number2 = np.random.uniform();

        X = [number1, number2]
        Y = 1 if number2 > number1 else 0
        yield X, [Y]
        
train_dataset = tf.data.Dataset.from_generator(
    data_generator, 
    output_types = (tf.float32, tf.int32),
    output_shapes=((2), (1))
)
train_dataset = train_dataset.batch(batch_size=30)


model = keras.Sequential([
    layers.Input(shape=(2)),
    layers.Dense(10, activation='relu'),
    layers.Dense(2, 'softmax')
])

model.summary()

model.compile(optimizer='adam',loss=keras.losses.SparseCategoricalCrossentropy(), metrics=['accuracy'])

# model.fit(train_X, train_Y, epochs = 5)

# model.evaluate(test_X, test_Y)



model.fit(
    train_dataset,
    steps_per_epoch=1000,
    epochs=4
)


converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

open("TfLiteModel.tflite", "wb").write(tflite_model)

os.system('xxd -i TfLiteModel.tflite > MnistModel.h')













