from lib2to3.pytree import convert
from turtle import shape
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras.datasets import mnist
import h5py

(train_X, train_Y), (test_X, test_Y) = mnist.load_data()
train_X = train_X/255.0
test_X = test_X/255.0

model = keras.Sequential([
    layers.Flatten(input_shape=(28,28)),
    layers.Dense(128, activation='relu'),
    layers.Dense(10)
])

model.summary()

model.compile(optimizer='adam',loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True), metrics=['accuracy'])

model.fit(train_X, train_Y, epochs = 5)

model.evaluate(test_X, test_Y)

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

open("TfLiteModel", "wb").write(tflite_model)










