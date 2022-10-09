# HIP

HIP is a C++ runtime API 。我们使用C++语言，可以调用HIP的API来进行编程。HIP可以运行在ROCm平台，也可以运行在CUDA平台。



## Cuda 兼容性

https://github.com/ROCm-Developer-Tools/HIP/blob/develop/docs/markdown/CUDA_Runtime_API_functions_supported_by_HIP.md

### CUDA转HIP

> A 卡和 N 卡在 warp size 上有差异，在对移植代码进行性能优化时也需要考虑。

**HIP提供了移植工具 [“hipify”](https://github.com/ROCm-Developer-Tools/HIPIFY)，可以帮助你将cuda源代码转换成HIP**，与原始的CUDA应用程序相比没有开销。

大多数情况下，通过 ”cuda“ 和 ”hip“字符的相互替换（e.g. cudaMalloc -> hipMalloc），可以在源码级别完成移植
