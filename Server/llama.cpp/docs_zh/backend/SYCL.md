# llama.cpp for SYCL

- [背景](#background)
- [推荐版本](#recommended-release)
- [新闻](#news)
- [操作系统](#os)
- [硬件](#hardware)
- [Docker](#docker)
- [Linux](#linux)
- [Windows](#windows)
- [环境变量](#environment-variable)
- [已知问题](#known-issues)
- [常见问题解答](#qa)
- [待办事项](#todo)

## 背景

**SYCL** 是一种高级并行编程模型，旨在提高开发者在各种硬件加速器（如CPU、GPU和FPGA）上编写代码的生产力。它是一种针对异构计算的单源语言，基于标准C++17。

**oneAPI** 是一个开放的生态系统和基于标准的规范，支持包括但不限于英特尔CPU、GPU和FPGA在内的多种架构。oneAPI生态系统的关键组件包括：

- **DPCPP** *(数据并行C++)*: oneAPI SYCL 的主要实现，包括 icpx/icx 编译器。
- **oneAPI 库**: 一系列针对多个领域的高性能优化库 *(例如 oneMKL - 数学内核库)*。
- **oneAPI LevelZero**: 一个高性能的低级接口，用于精细控制英特尔集成GPU和离散GPU。
- **Nvidia & AMD 插件**: 这些插件扩展了oneAPI的DPCPP支持，以支持Nvidia和AMD GPU目标上的SYCL。

### Llama.cpp + SYCL

Llama.cpp SYCL后端首先支持**Intel GPU**。基于SYCL的跨平台特性，它还支持其他厂商的GPU：Nvidia GPU (*AMD GPU即将支持*)。

当针对**Intel CPU**时，建议使用Llama.cpp的[Intel oneMKL](README.md#intel-onemkl)后端。

它与其他基于Llama.cpp BLAS路径（如*OpenBLAS, cuBLAS等*）有类似的设计。在早期工作阶段，使用了一个API的[SYCLomatic](https://github.com/oneapi-src/SYCLomatic)开源迁移工具（商业发布[Intel® DPC++ 兼容性工具](https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compatibility-tool.html))来完成这项工作。

## 推荐发布

由于没有在线CI，某些PR可能会破坏SYCL后端。

以下发布经过验证，质量良好：

|提交ID|标签|发布|验证平台|
|-|-|-|-|
|fb76ec31a9914b7761c1727303ab30380fd4f05c|b3038|[llama-b3038-bin-win-sycl-x64.zip](https://github.com/ggerganov/llama.cpp/releases/download/b3038/llama-b3038-bin-win-sycl-x64.zip) |Arc770/Linux/oneAPI 2024.1<br>MTL Arc GPU/Windows 11/oneAPI 2024.1|

## 新闻

- 2024.5
  - 性能提升：在Arc770上，lama-2-7b.Q4_0的吞吐量从34 tokens/s提升至37 tokens/s。
  - 成功验证了Arch Linux。

- 2024.4
  - 支持数据类型：GGML_TYPE_IQ4_NL, GGML_TYPE_IQ4_XS, GGML_TYPE_IQ3_XXS, GGML_TYPE_IQ3_S, GGML_TYPE_IQ2_XXS, GGML_TYPE_IQ2_XS, GGML_TYPE_IQ2_S, GGML_TYPE_IQ1_S, GGML_TYPE_IQ1_M。

- 2024.3
  - 发布Windows的二进制文件。
  - 发布博客：**使用 llama.cpp 在所有英特尔GPU上运行LLM**：[intel.com](https://www.intel.com/content/www/us/en/developer/articles/technical/run-llm-on-all-gpus-using-llama-cpp-artical.html) 或 [medium.com](https://medium.com/@jianyu_neo/run-llm-on-all-intel-gpus-using-llama-cpp-fd2e2dcbd9bd)。
  - 准备好新的基线：[tag b2437](https://github.com/ggerganov/llama.cpp/tree/b2437)。
  - 支持多卡：**--split-mode**: [none|layer]; 不支持 [row]，正在开发中。
  - 支持通过 **--main-gpu** 分配主GPU，替换 $GGML_SYCL_DEVICE。
  - 支持检测所有具有相同最高 **Max compute units** 级别的GPU。
  - 支持OPs
    - hardsigmoid
    - hardswish
    - pool2d

- 2024.1
  - 为英特尔GPU创建SYCL后端。
  - 支持Windows构建。

## 操作系统

| 操作系统 (OS)  | 状态  | 已验证 |
|--------------|------|-------|
| Linux        | 支持  | Ubuntu 22.04, Fedora Silverblue 39, Arch Linux |
| Windows      | 支持  | Windows 11 |

## 硬件

### 英特尔GPU

**已验证设备**

| 英特尔GPU                   | 状态  | 已验证型号                          |
|-----------------------------|------|------------------------------------|
| 英特尔数据中心Max系列        | 支持  | Max 1550, 1100                      |
| 英特尔数据中心Flex系列        | 支持  | Flex 170                           |
| 英特尔Arc系列                | 支持  | Arc 770, 730M, Arc A750             |
| 英特尔内置Arc GPU           | 支持  | Meteor Lake内置的Arc GPU            |
| 英特尔集成GPU               | 支持  | i5-1250P, i7-1260P, i7-1165G7的集成GPU |

*注意*：

- **内存**
  - 当运行大型模型时，设备内存是一个限制。在运行 `./bin/llama-cli` 时，加载的模型大小，*`llm_load_tensors: buffer_size`*，将在日志中显示。

  - 请确保主机GPU共享内存足够大，以便容纳模型的大小。例如，*llama-2-7b.Q4_0* 需要8.0GB以上用于集成GPU，4.0GB以上用于独立GPU。

- **执行单元 (EU)**
  - 如果iGPU少于80 EU，推理速度可能太慢，不适用于实际使用。

### 其他厂商GPU

**已验证设备**

| 英伟达GPU             | 状态 | 已验证型号 |
|------------------------|------|-----------|
| Ampere系列            | 支持 | A100, A4000 |
| Ampere系列 *(移动)*   | 支持 | RTX 40系列 |

## Docker
当前Docker构建选项仅限于*英特尔GPU*目标。

### 构建镜像
```sh
# 使用FP16
docker build -t llama-cpp-sycl --build-arg="GGML_SYCL_F16=ON" -f .devops/llama-cli-intel.Dockerfile .
```

*注意事项*：

要使用默认的FP32 *(比FP16替代方案慢)* 构建，可以从前面的命令中移除`--build-arg="GGML_SYCL_F16=ON"`参数。

您还可以使用`.devops/llama-server-intel.Dockerfile`，该文件构建了*"服务器"*替代方案。

### 运行容器

```sh
# 首先，查找所有的DRI卡
ls -la /dev/dri
# 然后，选择您想要使用的卡（例如，这里为 /dev/dri/card1）。
docker run -it --rm -v "$(pwd):/app:Z" --device /dev/dri/renderD128:/dev/dri/renderD128 --device /dev/dri/card1:/dev/dri/card1 llama-cpp-sycl -m "/app/models/YOUR_MODEL_FILE" -p "构建一个网站可以分成10个简单的步骤：" -n 400 -e -ngl 33
```

*注意事项*：
- Docker已在原生Linux上成功测试。WSL支持尚未验证。
- 您可能需要在**主机**机器上安装英特尔GPU驱动程序（请参阅[Linux配置](#linux)以获取详细信息）。

## Linux

### I. 环境配置

1. **安装GPU驱动**

  - **Intel GPU**

Intel数据中心GPU驱动程序的安装指南和下载页面可以在[获取Intel dGPU驱动](https://dgpu-docs.intel.com/driver/installation.html#ubuntu-install-steps)中找到。

*注意*: 对于客户端GPU（iGPU和Arc A系列），请参阅[客户端iGPU驱动安装](https://dgpu-docs.intel.com/driver/client/overview.html)。

安装完成后，将用户添加到`video`和`render`组。

```sh
sudo usermod -aG render $USER
sudo usermod -aG video $USER
```

*注意*: 登出/重新登录以使更改生效。

通过`clinfo`验证安装：

```sh
sudo apt install clinfo
sudo clinfo -l
```

示例输出：

```sh
Platform #0: Intel(R) OpenCL Graphics
 `-- Device #0: Intel(R) Arc(TM) A770 Graphics

Platform #0: Intel(R) OpenCL HD Graphics
 `-- Device #0: Intel(R) Iris(R) Xe Graphics [0x9a49]
```


- **Nvidia GPU**

为了通过SYCL针对Nvidia GPU进行目标定位，请确保已安装CUDA/CUBLAS本地要求*（见[README.md#cuda](README.md#cuda)）*。

2. **安装Intel® oneAPI Base toolkit**

- **对于Intel GPU**

可以从官方[Intel® oneAPI Base Toolkit](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html)页面获取基础工具包。

请遵循Linux下载和安装工具包的说明，并尽可能保持默认安装值不变，特别是安装路径*（默认为`/opt/intel/oneapi`）*。

以下指南/代码片段假设默认安装值。否则，请确保必要的更改已反映在适用的地方。

安装成功后，SYCL为可用的Intel设备启用，同时还有如oneAPI MKL等相关的库。

- **为Nvidia GPU添加支持**

**oneAPI插件**: 为了在Nvidia GPU上启用SYCL支持，请安装[Codeplay oneAPI Plugin for Nvidia GPUs](https://developer.codeplay.com/products/oneapi/nvidia/download)。用户还应确保插件版本与已安装的基础工具包版本匹配*(上一步骤)*，以实现无缝的"oneAPI on Nvidia GPU"设置。

**oneMKL for cuBlas**: 当前oneMKL版本 *(随oneAPI base-toolkit一起发货)* 不包含cuBLAS后端。因此，需要从源代码构建带有cuBLAS后端的[oneMKL](https://github.com/oneapi-src/oneMKL)，以便在Nvidia GPU上运行它。

```sh
git clone https://github.com/oneapi-src/oneMKL
cd oneMKL
cmake -B buildWithCublas -DCMAKE_CXX_COMPILER=icpx -DCMAKE_C_COMPILER=icx -DENABLE_MKLGPU_BACKEND=OFF -DENABLE_MKLCPU_BACKEND=OFF -DENABLE_CUBLAS_BACKEND=ON -DTARGET_DOMAINS=blas
cmake --build buildWithCublas --config Release
```


3. **验证安装和环境**

为了检查机器上可用的SYCL设备，请使用`sycl-ls`命令。
```sh
source /opt/intel/oneapi/setvars.sh
sycl-ls
```

- **Intel GPU**

当针对Intel GPU时，用户应预期在可用的SYCL设备中至少有一个level-zero设备。请确保至少有一个GPU存在，例如下述示例输出中的[`ext_oneapi_level_zero:gpu:0`]:

```
[opencl:acc:0] Intel(R) FPGA Emulation Platform for OpenCL(TM), Intel(R) FPGA Emulation Device OpenCL 1.2  [2023.16.10.0.17_160000]
[opencl:cpu:1] Intel(R) OpenCL, 13th Gen Intel(R) Core(TM) i7-13700K OpenCL 3.0 (Build 0) [2023.16.10.0.17_160000]
[opencl:gpu:2] Intel(R) OpenCL Graphics, Intel(R) Arc(TM) A770 Graphics OpenCL 3.0 NEO  [23.30.26918.50]
[ext_oneapi_level_zero:gpu:0] Intel(R) Level-Zero, Intel(R) Arc(TM) A770 Graphics 1.3 [1.3.26918]
```


- **Nvidia GPU**

同样，针对Nvidia GPU的用户应预期至少有一个SYCL-CUDA设备[`ext_oneapi_cuda:gpu`]，如下所示：
```
[opencl:acc:0] Intel(R) FPGA Emulation Platform for OpenCL(TM), Intel(R) FPGA Emulation Device OpenCL 1.2  [2023.16.12.0.12_195853.xmain-hotfix]
[opencl:cpu:1] Intel(R) OpenCL, Intel(R) Xeon(R) Gold 6326 CPU @ 2.90GHz OpenCL 3.0 (Build 0) [2023.16.12.0.12_195853.xmain-hotfix]
[ext_oneapi_cuda:gpu:0] NVIDIA CUDA BACKEND, NVIDIA A100-PCIE-40GB 8.0 [CUDA 12.2]
```

### II. 构建llama.cpp

#### Intel GPU

```sh
# 导出相关环境变量
source /opt/intel/oneapi/setvars.sh

# 使用MKL BLAS加速构建LLAMA，适用于Intel GPU

# 选项1：使用FP32（大多数情况下推荐，以获得更好的性能）
cmake -B build -DGGML_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx

# 选项2：使用FP16
cmake -B build -DGGML_SYCL=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx -DGGML_SYCL_F16=ON

# 构建所有二进制文件
cmake --build build --config Release -j -v
```

#### Nvidia GPU

```sh
# 导出相关环境变量
export LD_LIBRARY_PATH=/path/to/oneMKL/buildWithCublas/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=/path/to/oneMKL/buildWithCublas/lib:$LIBRARY_PATH
export CPLUS_INCLUDE_DIR=/path/to/oneMKL/buildWithCublas/include:$CPLUS_INCLUDE_DIR
export CPLUS_INCLUDE_DIR=/path/to/oneMKL/include:$CPLUS_INCLUDE_DIR

# 通过SYCL使用Nvidia BLAS加速构建LLAMA

# 选项1：使用FP32（大多数情况下推荐，以获得更好的性能）
cmake -B build -DGGML_SYCL=ON -DGGML_SYCL_TARGET=NVIDIA -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx

# 选项2：使用FP16
cmake -B build -DGGML_SYCL=ON -DGGML_SYCL_TARGET=NVIDIA -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx -DGGML_SYCL_F16=ON

# 构建所有二进制文件
cmake --build build --config Release -j -v
```

### III. 运行推理

1. 获取并准备模型

您可以参考[*准备和量化*](README.md#prepare-and-quantize)的一般指南来准备模型，或者简单地下载[llama-2-7b.Q4_0.gguf](https://huggingface.co/TheBloke/Llama-2-7B-GGUF/blob/main/llama-2-7b.Q4_0.gguf)模型作为示例。

2. 启用oneAPI运行环境

```sh
source /opt/intel/oneapi/setvars.sh
```

3. 列出设备信息

类似于本地的`sycl-ls`，可以使用以下命令查询可用的SYCL设备：

```sh
./build/bin/llama-ls-sycl-device
```

在具有1 *英特尔CPU* 和1 *英特尔GPU* 的系统上，此类日志的示例可能如下所示：

```plaintext
found 6 SYCL devices:
|  |                  |                                             |Compute   |Max compute|Max work|Max sub|               |
|ID|       Device Type|                                         Name|capability|units      |group   |group  |Global mem size|
|--|------------------|---------------------------------------------|----------|-----------|--------|-------|---------------|
| 0|[level_zero:gpu:0]|               Intel(R) Arc(TM) A770 Graphics|       1.3|        512|    1024|     32|    16225243136|
| 1|[level_zero:gpu:1]|                    Intel(R) UHD Graphics 770|       1.3|         32|     512|     32|    53651849216|
| 2|    [opencl:gpu:0]|               Intel(R) Arc(TM) A770 Graphics|       3.0|        512|    1024|     32|    16225243136|
| 3|    [opencl:gpu:1]|                    Intel(R) UHD Graphics 770|       3.0|         32|     512|     32|    53651849216|
| 4|    [opencl:cpu:0]|         13th Gen Intel(R) Core(TM) i7-13700K|       3.0|         24|    8192|     64|    67064815616|
| 5|    [opencl:acc:0]|               Intel(R) FPGA Emulation Device|       1.2|         24|67108864|     64|    67064815616|
```

| 属性              | 备注                                                        |
|-------------------|-----------------------------------------------------------|
| compute capability 1.3 | Level-zero驱动程序/运行时，推荐                          |
| compute capability 3.0 | OpenCL驱动程序/运行时，大多数情况下比level-zero慢        |

4. 启动推理

有两种设备选择模式：

- 单个设备：使用用户指定的单个设备目标。
- 多个设备：自动选择具有相同最大计算单元的设备。

| 设备选择 | 参数 |
|-----------|------|
| 单个设备 | --split-mode none --main-gpu DEVICE_ID |
| 多个设备 | --split-mode layer (默认) |

示例：

- 使用设备0：

```sh
ZES_ENABLE_SYSMAN=1 ./build/bin/llama-cli -m models/llama-2-7b.Q4_0.gguf -p "Building a website can be done in 10 simple steps:" -n 400 -e -ngl 33 -sm none -mg 0
```

或通过脚本运行：

```sh
./examples/sycl/run_llama2.sh 0
```

- 使用多个设备：

```sh
ZES_ENABLE_SYSMAN=1 ./build/bin/llama-cli -m models/llama-2-7b.Q4_0.gguf -p "Building a website can be done in 10 simple steps:" -n 400 -e -ngl 33 -sm layer
```

否则，你可以运行脚本：

```sh
./examples/sycl/run_llama2.sh
```

*注意事项：*

- 执行时，请在输出日志中验证所选设备ID，例如可以显示如下：

```sh
detect 1 SYCL GPUs: [0] with top Max compute units:512
```

或

```sh
use 1 SYCL GPUs: [0] with Max compute units:512
```

## Windows

### I. 环境设置

1. 安装GPU驱动程序

Intel GPU驱动程序的说明指南和下载页面可以在以下链接找到：[获取Intel GPU驱动程序](https://www.intel.com/content/www/us/en/products/docs/discrete-gpus/arc/software/drivers.html)。

2. 安装Visual Studio

如果您已经安装了较新的Microsoft Visual Studio版本，您可以跳过此步骤。否则，请参考[Microsoft Visual Studio](https://visualstudio.microsoft.com/)的官方下载页面。

3. 安装Intel® oneAPI Base工具包

基础工具包可以从[Intel® oneAPI Base Toolkit](https://www.intel.com/content/www/us/en/developer/tools/oneapi/base-toolkit.html)的官方页面获取。

请按照Windows版本的下载和安装指南进行操作，并尽可能保持默认安装值不变，特别是安装路径（默认为`C:\Program Files (x86)\Intel\oneAPI`）。

以下指南/代码片段假设默认安装值。否则，请确保在适用处进行了必要的更改。

b. 启用oneAPI运行环境：

- 在搜索栏中输入“oneAPI”，然后打开`Intel oneAPI命令提示符 for Intel 64 for Visual Studio 2022`应用程序。

- 在命令提示符中，使用以下命令启用运行环境：
```bash
"C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64
```

c. 验证安装

在oneAPI命令行中，运行以下命令以打印可用的SYCL设备：

```bash
sycl-ls
```

应该会显示一个或多个**[ext_oneapi_level_zero:gpu]**作为**level-zero** GPU设备。以下是一个检测到**intel Iris Xe** GPU作为Level-zero SYCL设备的输出示例：

输出（示例）：
```bash
[opencl:acc:0] Intel(R) FPGA Emulation Platform for OpenCL(TM), Intel(R) FPGA Emulation Device OpenCL 1.2  [2023.16.10.0.17_160000]
[opencl:cpu:1] Intel(R) OpenCL, 11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz OpenCL 3.0 (Build 0) [2023.16.10.0.17_160000]
[opencl:gpu:2] Intel(R) OpenCL Graphics, Intel(R) Iris(R) Xe Graphics OpenCL 3.0 NEO  [31.0.101.5186]
[ext_oneapi_level_zero:gpu:0] Intel(R) Level-Zero, Intel(R) Iris(R) Xe Graphics 1.3 [1.3.28044]
```

```markdown
4. 安装构建工具

a. 下载并安装Windows版本的CMake: https://cmake.org/download/ (CMake也可以通过Visual Studio Installer安装)
b. 新的Visual Studio默认会安装Ninja。 (如果未安装，请手动安装: https://ninja-build.org/)


### II. 构建 llama.cpp

在oneAPI命令行窗口中，进入llama.cpp主目录，并运行以下命令：

```bash
@call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64 --force

# 选项1: 使用FP32 (在大多数情况下推荐，以获得更好的性能)
cmake -B build -G "Ninja" -DGGML_SYCL=ON -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=icx  -DCMAKE_BUILD_TYPE=Release

# 选项2: 或者使用FP16
cmake -B build -G "Ninja" -DGGML_SYCL=ON -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=icx  -DCMAKE_BUILD_TYPE=Release -DGGML_SYCL_F16=ON

cmake --build build --config Release -j
```

否则，运行`win-build-sycl.bat`包装器，它封装了前面的指令：
```bash
.\examples\sycl\win-build-sycl.bat
```

或者，使用CMake预设来构建：
```bash
cmake --preset x64-windows-sycl-release
cmake --build build-x64-windows-sycl-release -j --target llama-cli

cmake -DGGML_SYCL_F16=ON --preset x64-windows-sycl-release
cmake --build build-x64-windows-sycl-release -j --target llama-cli

cmake --preset x64-windows-sycl-debug
cmake --build build-x64-windows-sycl-debug -j --target llama-cli
```

或者，您可以使用Visual Studio将llama.cpp文件夹作为CMake项目打开。在编译项目之前，选择sycl CMake预设(`x64-windows-sycl-release`或`x64-windows-sycl-debug`)。

*注意事项:* 

- 在最小实验设置的情况下，用户可以通过`cmake --build build --config Release -j --target llama-cli`仅构建推理可执行文件。
```
```

### III. 运行推理

1. 获取和准备模型

您可以使用[*准备和量化*](README#prepare-and-quantize)指南的一般指导进行模型准备，或者简单地下载[llama-2-7b.Q4_0.gguf](https://huggingface.co/TheBloke/Llama-2-7B-GGUF/blob/main/llama-2-7b.Q4_0.gguf)模型作为示例。

2. 启用oneAPI运行环境

在oneAPI命令行窗口中，运行以下命令并进入llama.cpp目录：
```
"C:\Program Files (x86)\Intel\oneAPI\setvars.bat" intel64
```

3. 列出设备信息

类似于本机`sycl-ls`，可以查询可用的SYCL设备，如下所示：

```build\bin\ls-sycl-device.exe
```

在具有1 *Intel CPU*和1 *Intel GPU*的系统上，该命令的输出如下所示：
```found 6 SYCL devices:
|  |                  |                                             |Compute   |Max compute|Max work|Max sub|               |
|ID|       Device Type|                                         Name|capability|units      |group   |group  |Global mem size|
|--|------------------|---------------------------------------------|----------|-----------|--------|-------|---------------|
| 0|[level_zero:gpu:0]|               Intel(R) Arc(TM) A770 Graphics|       1.3|        512|    1024|     32|    16225243136|
| 1|[level_zero:gpu:1]|                    Intel(R) UHD Graphics 770|       1.3|         32|     512|     32|    53651849216|
| 2|    [opencl:gpu:0]|               Intel(R) Arc(TM) A770 Graphics|       3.0|        512|    1024|     32|    16225243136|
| 3|    [opencl:gpu:1]|                    Intel(R) UHD Graphics 770|       3.0|         32|     512|     32|    53651849216|
| 4|    [opencl:cpu:0]|         13th Gen Intel(R) Core(TM) i7-13700K|       3.0|         24|    8192|     64|    67064815616|
| 5|    [opencl:acc:0]|               Intel(R) FPGA Emulation Device|       1.2|         24|67108864|     64|    67064815616|

```

| 属性              | 备注                                                      |
|-------------------|----------------------------------------------------------|
| compute capability 1.3 | Level-zero 运行时间，推荐                          |

| 计算能力 3.0 | OpenCL 运行时间，大多数情况下比 0 级别慢 |

4. 启动推理

有两种设备选择模式：

- 单个设备：使用用户分配的一个设备。
- 多个设备：自动选择具有最大最大计算单元的设备。

| 设备选择 | 参数                                 |
|-----------|--------------------------------------|
| 单个设备  | --split-mode none --main-gpu DEVICE_ID |
| 多个设备  | --split-mode layer (默认)             |

示例：

- 使用设备 0：

```
build\bin\llama-cli.exe -m models\llama-2-7b.Q4_0.gguf -p "建立网站可以分 10 步简单完成：\n第一步：" -n 400 -e -ngl 33 -s 0 -sm none -mg 0
```

- 使用多个设备：

```
build\bin\llama-cli.exe -m models\llama-2-7b.Q4_0.gguf -p "建立网站可以分 10 步简单完成：\n第一步：" -n 400 -e -ngl 33 -s 0 -sm layer
```

否则，运行以下包装脚本：

```
.\examples\sycl\win-run-llama2.bat
```

注意：

- 执行时，请在输出日志中验证所选设备 ID，例如如下显示：

```sh
detect 1 SYCL GPUs: [0] with top Max compute units:512
```
或者
```sh
use 1 SYCL GPUs: [0] with Max compute units:512
```

## 环境变量

#### 构建

| 名称                 | 值                             | 功能                                    |
|----------------------|---------------------------------|-----------------------------------------|
| GGML_SYCL            | ON (必选)                    | 启用使用SYCL代码路径的构建。           |
| GGML_SYCL_TARGET     | INTEL *(默认)* \| NVIDIA       | 设置SYCL目标设备类型。            |
| GGML_SYCL_F16       | OFF *(默认)* \|ON *(可选)* | 启用使用SYCL代码路径的FP16构建。      |
| CMAKE_C_COMPILER     | icx                               | 设置*icx*编译器用于SYCL代码路径。      |
| CMAKE_CXX_COMPILER   | icpx *(Linux)*, icx *(Windows)* | 设置`icpx/icx`编译器用于SYCL代码路径。 |

#### 运行时

| 名称              | 值            | 功能                                                                                                                  |
|-------------------|----------------|---------------------------------------------------------------------------------------------------------------------------|
| GGML_SYCL_DEBUG   | 0 (默认) or 1 | 通过宏启用日志功能：GGML_SYCL_DEBUG                                                                             |
| ZES_ENABLE_SYSMAN | 0 (默认) or 1 | 支持通过sycl::aspect::ext_intel_free_memory获取GPU的空闲内存。<br>当--split-mode = layer时建议使用 |

## 已知问题

- `Split-mode:[row]` 不受支持。

## Q&A

- 错误：`error while loading shared libraries: libsycl.so.7: cannot open shared object file: No such file or directory`。

  - 可能原因：oneAPI安装不可用或未设置环境变量。
  - 解决方案：安装 *oneAPI base toolkit* 并通过以下方式启用其环境变量：`source /opt/intel/oneapi/setvars.sh`。

- 一般编译器错误：

  - 删除 **build** 文件夹或尝试干净构建。

- 在Linux上安装GPU驱动程序后，我 **无法** 看到 `[ext_oneapi_level_zero:gpu]`。

  请通过 `sudo sycl-ls` 进行双重检查。

  如果它在列表中，请将视频/渲染组添加到您的用户，然后 **注销/登录** 或重启系统：

  ```bash
  sudo usermod -aG render $USER
  sudo usermod -aG video $USER
  ```

  否则，请重新检查GPU驱动程序的安装步骤。

### **GitHub贡献**：

请在issue/PR标题中添加 **[SYCL]** 前缀/标签，以便SYCL团队能够及时检查/解决它们。

## TODO

- 支持多卡片运行时行层分割。


人生其实就像天气，有阳光明媚的日子，也有阴云密布的日子，偶尔还会有阵雨。这是事物的自然顺序。生活并不简单，但我们应该尽可能地去简化它。

