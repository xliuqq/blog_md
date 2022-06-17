[toc]

# databricks/spark-deep-learning

提供一些high-level的API，把底层的模型进行组件化，同时它期望可以兼容底层深度式学习框架。



# Yahoo/TensorflowOnSpark 

TensorflowOnSpark Pipeline开发了两个API，一个是TFEstimator，另一个是TFModel，提供了这两个之后，可以直接把它们集成到spark-deep-learning pipeline里面，进行进一步的训练。