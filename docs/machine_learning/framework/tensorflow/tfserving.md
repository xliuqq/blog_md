# TFServing



## 接收图片base64作为输入

```python
def preprocess_input(base64_input_bytes):    
    def decode_bytes(img_bytes):
        img = tf.image.decode_jpeg(img_bytes, channels=3)
        img = tf.image.resize(img, MODEL_INPUT_SHAPE)
        img = tf.image.convert_image_dtype(img, MODEL_INPUT_DTYPE)
        return img

    base64_input_bytes = tf.reshape(base64_input_bytes, (-1,))
    return tf.map_fn(lambda img_bytes:
                     decode_bytes(img_bytes),
                     elems=base64_input_bytes,                     
                     fn_output_signature=MODEL_INPUT_DTYPE)

inputs = tf.keras.layers.Input(shape=(), dtype=tf.string, name='b64_input_bytes')
x = tf.keras.layers.Lambda(preprocess_input, name='decode_image_bytes')(inputs)
x = my_custom_model(x)
serving_model = tf.keras.Model(inputs, x)

tf.saved_model.save(serving_model, './my_serving_model')
```