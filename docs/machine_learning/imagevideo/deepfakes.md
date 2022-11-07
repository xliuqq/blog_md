# deepfakes

“Deepfakes”一词由“Deep learning”和“Fake”合并产生，指各种换脸（face-swapping）和面部重现（face-reenactment）方法，包括使用最先进的计算机视觉和深度学习技术的方法。

Deepfakes生成的假脸主要包括以下两大类：

1. **换脸（Face-swapping）**：将视频或图像中的人脸自动地替换为其他人脸，身份发生了改变。
2. **面部重现（Face-reenactment）**：将源角色的表情和姿势转移到目标视频中，而目标主体的身份保持不变。

## 1. 换脸（Face-swapping）

### Original Deepfakes

**原理**：使用**两对encoder-decoder**，但在训练时两个encoder共享参数，但分别训练decoder。使用时将原始脸A输入Encoder，再连接B的Decoder，解码即可实现将B的脸换为A的脸。

**缺点：劳动密集和消耗大量计算资源**。该方法只有在拥有大量目标人物图片和视频素材（300到2000张）作为训练数据的前提下才能达到相对理想的效果。

**实现**：github上与该方法最贴近且star最高的代码实现 https://github.com/deepfakes/faceswap

### Faceswap-GAN

原理：在上一个基于Autoencoder的方法上加入**adversarial loss和perceptual loss**便可用GAN实现换脸。

实现：https://github.com/shaoanlu/faceswap-GAN

### 3D face FaceSwap

原理：通过检测人脸landmarks提取人脸区域，通过这些landmarks用blendshapes可以fit一个3D模板模型，用输入图像的纹理最小化投影形状和定位landmarks的差异，这个模型可以反映射到目标图像上，最后，将渲染模型与图像混合，并进行颜色校正。实现在计算上是轻量级的，并且可以在CPU上有效地运行。

实现：https://github.com/MarekKowalski/FaceSwap

### Fast Face-swap Using CNN(ICCV2017)

受图像风格迁移（artistic style transfer）的影响，该方法将A的人脸姿态和表情作为content，B的身份作为style，生成时保持A的content的同时学习B的style。

### On Face Segmentation, Face Swapping, and Face Perception (2018)

原理：基于一个全卷积神经网络（FCN）人脸分割算法来实现换脸。

实现：https://github.com/YuvalNirkin/face_swap

### FSGAN: Subject Agnostic Face Swapping and Reenactment (ICCV2019)

原理：FSGAN是subject agnostic（不可知的），可以应用于成对的脸，而不需要对这些面孔进行训练。该网络采用了一种新的Poisson blending loss (泊松混合损耗)，将Poisson optimization (泊松优化)与perceptual loss相结合。

实现：https://github.com/YuvalNirkin/fsgan

### FaceShifter: Towards High Fidelity And Occlusion Aware Face Swapping (arXiv2019)

实现：https://github.com/mindslab-ai/faceshifter

### HifiFace: 3D Shape and Semantic Prior Guided High Fidelity Face Swapping（arXiv 2021）

实现：https://github.com/mindslab-ai/hififace

### DeepFaceLab

> DeepFaceLab is a tool that utilizes machine learning to replace faces in videos. Includes prebuilt ready to work standalone Windows 7,8,10 binary.

https://github.com/iperov/DeepFaceLab



## 2. 面部重现（Face-reenactment）

### Face2face: Real-time face capture and reenactment of rgb videos (CVPR2016)

**实时面部重建，借助 dlib 和 OpenCV**，首先人脸检测器检测出源图片中的人脸、找到人脸上的关键标记点，然后fit两个3D模型，再使用针对人脸的pix2pix转换模型把关键标记点转换为目标人脸图像。**效果也一般般，用做对比实验。**

### Few-Shot Adversarial Learning of Realistic Neural Talking Head Models(arXiv2019 Samsung AI Center)

特点：学习新的头部模型仅需要少量训练素材（8帧图像甚的至至单帧图像）和少量训练时间。该系统不需要大量的训练示例，而且系统只需要看一次图片就可以运行。



## 3.  Deepfake Detection

https://zhuanlan.zhihu.com/p/92474937



## 4. 数据集

https://zhuanlan.zhihu.com/p/92853899