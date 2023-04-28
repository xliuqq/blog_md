# TFServing

## 保存的模型

```python
import tempfile

# 版本路径是自行拼接
MODEL_DIR = tempfile.gettempdir()
version = 1
export_path = os.path.join(MODEL_DIR, str(version))
print('export_path = {}\n'.format(export_path))

tf.keras.models.save_model(
    model,
    export_path,
    overwrite=True,
    include_optimizer=True,
    save_format=None,
    signatures=None,
    options=None
)

print('\nSaved model:')
!ls -l {export_path}
```

路径格式

```shell
ls /tmpfs/tmp/1

drwxr-xr-x 2 kbuilder kbuilder  4096 Mar 15 10:34 assets
-rw-rw-r-- 1 kbuilder kbuilder    57 Mar 15 10:34 fingerprint.pb
-rw-rw-r-- 1 kbuilder kbuilder  8757 Mar 15 10:34 keras_metadata.pb
-rw-rw-r-- 1 kbuilder kbuilder 77904 Mar 15 10:34 saved_model.pb
drwxr-xr-x 2 kbuilder kbuilder  4096 Mar 15 10:34 variables
```



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