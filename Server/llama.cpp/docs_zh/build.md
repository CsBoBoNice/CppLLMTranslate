# 在本地构建llama.cpp

**获取代码：**

```bash
git clone https://github.com/ggerganov/llama.cpp
cd llama.cpp
```

为了构建llama.cpp，您有四种不同的选择。

- 使用`make`：

  - 在Linux或MacOS上：

      ```bash
      make
      ```

  - 在Windows上：

    1. 下载最新版本的[w64devkit](https://github.com/skeeto/w64devkit/releases)。
    2. 在您的电脑上解压`w64devkit`。
    3. 运行`w64devkit.exe`。
    4. 使用`cd`命令到达`llama.cpp`文件夹。
    5. 从这里您可以运行：
        ```bash
        make
        ```

  - 注意：

    - 为了加快编译速度，请添加`-j`参数以并行运行多个任务。例如，`make -j 8`将并行运行8个任务。
    - 为了加快重复编译，请安装[ccache](https://ccache.dev/)。
    - 为了调试构建，请运行`make LLAMA_DEBUG=1`

- 使用`CMake`：

  ```bash
  cmake -B build
  cmake --build build --config Release
  ```

  **注意**：

    - 为了加快编译速度，请添加`-j`参数以并行运行多个任务。例如，`cmake --build build --config Release -j 8`将并行运行8个任务。
    - 为了加快重复编译，请安装[ccache](https://ccache.dev/)。
    - 对于调试构建，有两种情况：

      1. 单配置生成器（例如默认值为`Unix Makefiles`；请注意，它们只是忽略`--config`标志）：

      ```bash
      cmake -B build -DCMAKE_BUILD_TYPE=Debug
      cmake --build build
      ```

      2. 多配置生成器（`-G`参数设置为Visual Studio，XCode等）：

      ```bash
      cmake -B build -G "Xcode"
      cmake --build build --config Debug
      ```

- 使用`gmake`（FreeBSD）：

    1. 安装并激活[FreeBSD中的DRM](https://wiki.freebsd.org/Graphics)
    2. 将您的用户添加到**video**组
    3. 安装编译依赖项。

        ```bash
        sudo pkg install gmake automake autoconf pkgconf llvm15 openblas

        gmake CC=/usr/local/bin/clang15 CXX=/usr/local/bin/clang++15 -j4
        ```

## Metal 构建过程

在 macOS 上，Metal 默认启用。使用 Metal 使得计算运行在 GPU 上。要在编译时禁用 Metal 构建，请使用 `GGML_NO_METAL=1` 标志或 `GGML_METAL=OFF` CMake 选项。

构建时启用 Metal 支持的情况下，可以通过 `--n-gpu-layers|-ngl 0` 命令行参数显式禁用 GPU 推理。

## BLAS 构建过程

使用 BLAS 支持构建程序可能会导致使用大于 32 的批处理大小（默认为 512）的提示处理性能提升。仅使用 CPU 的 BLAS 实现不会影响正常生成性能。使用涉及 GPU 的 BLAS 实现时，例如 cuBLAS、hipBLAS，我们可能会看到生成性能的改进。目前有几个不同的 BLAS 实现可用于构建和使用：

### Accelerate 框架：

这仅在 Mac 电脑上可用，并且默认启用。您可以使用正常的构建指令进行构建。

### OpenBLAS：

这提供仅使用 CPU 的 BLAS 加速。请确保您的机器上已安装 OpenBLAS。

- 使用 `make`：
  - 在 Linux 上：
    ```bash
    make GGML_OPENBLAS=1
    ```

  - 在 Windows 上：

    1. 下载 [w64devkit](https://github.com/skeeto/w64devkit/releases) 的最新 Fortran 版本。
    2. 下载 [OpenBLAS for Windows](https://github.com/xianyi/OpenBLAS/releases) 的最新版本。
    3. 在您的电脑上解压 `w64devkit`。
    4. 从您刚刚下载的 OpenBLAS zip 文件中复制 `libopenblas.a`，位于 `lib` 文件夹内，在 `w64devkit\x86_64-w64-mingw32\lib`。
    5. 从相同的 OpenBLAS zip 文件中复制 `include` 文件夹的内容，位于 `w64devkit\x86_64-w64-mingw32\include`。
    6. 运行 `w64devkit.exe`。
    7. 使用 `cd` 命令到达 `llama.cpp` 文件夹。
    8. 从这里您可以运行：

        ```bash
        make GGML_OPENBLAS=1
        ```

- 使用 Linux 上的 `CMake`：

    ```bash
    cmake -B build -DGGML_BLAS=ON -DGGML_BLAS_VENDOR=OpenBLAS
    cmake --build build --config Release
    ```

### BLIS

请查阅 [BLIS.md](./backend/BLIS.md) 获取更多信息。

### SYCL

SYCL 是一种高级编程模型，用于提高在各种硬件加速器上的编程效率。

基于 SYCL 的 llama.cpp 用于 **支持英特尔 GPU**（数据中心 Max 系列、Flex 系列、Arc 系列、内置 GPU 和集成 GPU）。 

有关详细信息，请参阅 [基于 SYCL 的 llama.cpp](./backend/SYCL.md)。

### Intel oneMKL

通过 oneAPI 编译器构建将使 avx_vnni 指令集可用于不支持 avx512 和 avx512_vnni 的英特尔处理器。请注意，此构建配置 **不支持英特尔 GPU**。若要支持英特尔 GPU，请参阅 [基于 SYCL 的 llama.cpp](./backend/SYCL.md)。

- 使用手动 oneAPI 安装：
  默认情况下，`GGML_BLAS_VENDOR` 被设置为 `Generic`，因此如果您已经源码了英特尔环境脚本，并在 cmake 中指定 `-DGGML_BLAS=ON`，则 Blas 的 MKL 版本将自动被选择。否则，请安装 oneAPI 并按照以下步骤操作：
    ```bash
    source /opt/intel/oneapi/setvars.sh # 如果在 oneapi-basekit docker 镜像中，您可以跳过此步骤，只有手动安装时需要
    cmake -B build -DGGML_BLAS=ON -DGGML_BLAS_VENDOR=Intel10_64lp -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx -DGGML_NATIVE=ON
    cmake --build build --config Release
    ```

- 使用 oneAPI docker 镜像：
  如果您不想源码环境变量并手动安装 oneAPI，您也可以使用英特尔 docker 容器：[oneAPI-basekit](https://hub.docker.com/r/intel/oneapi-basekit)。然后，您可以使用上述命令。

请查阅 [优化和运行 LLaMA2 在英特尔® CPU 上](https://www.intel.com/content/www/us/en/content-details/791610/optimizing-and-running-llama2-on-intel-cpu.html) 获取更多信息。

### CUDA

这使用Nvidia GPU的CUDA核心提供GPU加速。请确保已安装CUDA工具包。您可以从Linux发行版的包管理器中下载它（例如`apt install nvidia-cuda-toolkit`）或从这里下载：[CUDA Toolkit](https://developer.nvidia.com/cuda-downloads)。

对于Jetson用户，如果您使用的是Jetson Orin，您可以尝试这个：[官方支持](https://www.jetson-ai-lab.com/tutorial_text-generation.html)。如果您使用的是旧型号（nano/TX2），在编译之前需要进行一些额外的操作。

- 使用`make`：
  ```bash
  make GGML_CUDA=1
  ```
- 使用`CMake`：

  ```bash
  cmake -B build -DGGML_CUDA=ON
  cmake --build build --config Release
  ```

可以使用环境变量`CUDA_VISIBLE_DEVICES`来指定哪些GPU将被使用。以下编译选项也可以用来调整性能：

| 选项                          | 合法值               | 默认值 | 描述                                                                                                                                                                                                                                                                             |
|-------------------------------|------------------------|---------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| GGML_CUDA_FORCE_DMMV          | 布尔值                | false   | 强制使用去量化 + 矩阵向量乘法内核，而不是使用在量化数据上执行矩阵向量乘法的内核。默认情况下，根据计算能力（MMVQ用于6.1/Pascal/GTX 1000或更高）做出决定。不影响k-量化。 |
| GGML_CUDA_DMMV_X              | 正整数 >= 32           | 32      | 每次迭代由CUDA去量化 + 矩阵向量乘法内核处理的x方向中的值数。增加此值可以提高快速GPU上的性能。强烈推荐使用2的幂。不影响k-量化。 |
| GGML_CUDA_MMV_Y               | 正整数 | 1       | CUDA mul mat vec 核心的 y 方向块大小。增加此值可以提高快速 GPU 上的性能。建议使用 2 的幂。                                                                                           |
| GGML_CUDA_FORCE_MMQ           | 布尔值  | false   | 强制使用自定义矩阵乘法内核进行量化模型，即使没有可用的 int8 张量核心实现（影响 V100、RDNA3）。在支持 int8 张量核心的 GPU 上默认启用 MMQ 内核。启用 MMQ 强制后，对于大型批处理大小，速度会更慢，但 VRAM 消耗会更低。                                                                 |
| GGML_CUDA_FORCE_CUBLAS        | 布尔值  | false   | 强制使用 FP16 cuBLAS 而不是自定义矩阵乘法内核进行量化模型                                                                                                                                  |
| GGML_CUDA_F16                 | 布尔值  | false   | 如果启用，使用半精度浮点运算进行 CUDA 解量化和 mul mat vec 核心以及 q4_1 和 q5_1 矩阵矩阵乘法内核。可以相对提高较新 GPU 的性能。                                                                   |
| GGML_CUDA_KQUANTS_ITER        | 1 或 2 | 2       | 每次迭代和每个 CUDA 线程处理 Q2_K 和 Q6_K 量化格式的值数量。将此值设置为 1 可以提高慢速 GPU 的性能。                                                                                       |
| GGML_CUDA_PEER_MAX_BATCH_SIZE | 正整数 | 128     | 为启用多个 GPU 之间 peer 访问的最大批处理大小。peer 访问需要 Linux 或 NVLink。当使用 NVLink 时，为大型批处理大小启用 peer 访问可能是有益的。                                                                     |
| GGML_CUDA_FA_ALL_QUANTS       | 布尔值                | false   | 为FlashAttention CUDA内核编译支持所有KV缓存量化类型（组合）。可以更精细地控制KV缓存大小，但编译时间会很长。                                                                                                  |

### hipBLAS

这为支持HIP的AMD GPU提供BLAS加速。请确保已安装ROCm。
您可以从您的Linux发行版的软件包管理器或从这里下载：[ROCm快速入门（Linux）](https://rocm.docs.amd.com/projects/install-on-linux/en/latest/tutorial/quick-start.html#rocm-install-quick)。
- 使用`make`:
  ```bash
  make GGML_HIPBLAS=1
  ```
- 使用`CMake` for Linux（假设使用兼容gfx1030的AMD GPU）:
  ```bash
  HIPCXX="$(hipconfig -l)/clang" HIP_PATH="$(hipconfig -R)" \
      cmake -S . -B build -DGGML_HIPBLAS=ON -DAMDGPU_TARGETS=gfx1030 -DCMAKE_BUILD_TYPE=Release \
      && cmake --build build --config Release -- -j 16
  ```
  在Linux上，也可以通过设置`-DGGML_HIP_UMA=ON`来使用统一内存架构（UMA），以在CPU和集成GPU之间共享主内存。
  然而，这会损害非集成GPU的性能（但允许与集成GPU一起工作）。
  注意，如果您遇到以下错误：
  ```
  clang: error: cannot find ROCm device library; provide its path via '--rocm-path' or '--rocm-device-lib-path', or pass '-nogpulib' to build without ROCm device library
  ```
  尝试在`HIP_PATH`下搜索包含文件`oclc_abi_version_400.bc`的目录。然后，将以下内容添加到命令的起始部分：`HIP_DEVICE_LIB_PATH=<您刚刚找到的目录>`，例如：
  ```bash
  HIPCXX="$(hipconfig -l)/clang" HIP_PATH="$(hipconfig -p)" \
  HIP_DEVICE_LIB_PATH=<您刚刚找到的目录> \
      cmake -S . -B build -DGGML_HIPBLAS=ON -DAMDGPU_TARGETS=gfx1030 -DCMAKE_BUILD_TYPE=Release \
      && cmake --build build -- -j 16
  ```

- 使用`make`（针对gfx1030的目标，使用16个CPU线程进行构建）:
  ```bash
  make -j16 GGML_HIPBLAS=1 GGML_HIP_UMA=1 AMDGPU_TARGETS=gfx1030
  ```

- 在Windows上使用`CMake`（使用x64本地工具命令提示符的VS，并假定有一个兼容gfx1100的AMD GPU）:
  ```bash
  set PATH=%HIP_PATH%\bin;%PATH%
  cmake -S . -B build -G Ninja -DAMDGPU_TARGETS=gfx1100 -DGGML_HIPBLAS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
  cmake --build build
  ```
  确保将`AMDGPU_TARGETS`设置为你要编译的GPU架构。上面的例子使用`gfx1100`，对应于Radeon RX 7900XTX/XT/GRE。你可以在这里找到目标列表[链接](https://llvm.org/docs/AMDGPUUsage.html#processors)。
  通过将`rocminfo | grep gfx | head -1 | awk '{print $2}'`的最重要的版本信息与处理器列表匹配，找到你的gpu版本字符串，例如`gfx1035`映射到`gfx1030`。


环境变量[`HIP_VISIBLE_DEVICES`](https://rocm.docs.amd.com/en/latest/understand/gpu_isolation.html#hip-visible-devices)可用于指定哪些GPU将被使用。
如果您的GPU未得到官方支持，您可以使用环境变量[`HSA_OVERRIDE_GFX_VERSION`]将其设置为类似GPU的值，例如RDNA2上的10.3.0（例如`gfx1030`、`gfx1031`或`gfx1035`），或RDNA3上的11.0.0。
以下编译选项也可以用于调整性能（是的，它们指的是CUDA，而不是HIP，因为它们使用与上面的cuBLAS版本相同的代码）：

| 选项                 | 合法值           | 默认 | 描述                                                                                                                                                                                                                                    |
|------------------------|------------------------|---------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| GGML_CUDA_DMMV_X       | 大于等于32的正整数 | 32      | 每次迭代中HIP去量化+矩阵向量乘法内核处理的x方向值数量。增加此值可以提高快速GPU的性能。强烈建议使用2的幂。不影响k-量化。 |
| GGML_CUDA_MMV_Y        | 正整数           | 1       | HIP 矩阵乘法和向量核在y方向上的块大小。增加此值可以提高快速GPU上的性能。推荐使用2的幂。不会影响k-量化。           |
| GGML_CUDA_KQUANTS_ITER | 1或2             | 2       | 每次迭代和每个HIP线程处理的Q2_K和Q6_K量化格式的值数。将此值设置为1可以提高慢速GPU的性能。                           |

### Vulkan

**使用Docker**：

您不需要安装Vulkan SDK。它将被安装在容器内。

```sh
# 构建镜像
docker build -t llama-cpp-vulkan -f .devops/llama-cli-vulkan.Dockerfile .

# 然后，使用它：
docker run -it --rm -v "$(pwd):/app:Z" --device /dev/dri/renderD128:/dev/dri/renderD128 --device /dev/dri/card1:/dev/dri/card1 llama-cpp-vulkan -m "/app/models/YOUR_MODEL_FILE" -p "构建一个网站可以分成10个简单的步骤：" -n 400 -e -ngl 33
```

**不使用Docker**：

首先，您需要确保已安装[Vulkan SDK](https://vulkan.lunarg.com/doc/view/latest/linux/getting_started_ubuntu.html)

例如，在Ubuntu 22.04 (jammy)上，使用以下命令：

```bash
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -
wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list https://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
apt update -y
apt-get install -y vulkan-sdk
# 要验证安装，使用以下命令：
vulkaninfo
```

或者您的包管理器可能能够提供适当的库。
例如，对于Ubuntu 22.04，您可以使用`libvulkan-dev`代替。
对于Fedora 40，您可以安装`vulkan-devel`、`glslc`和`glslang`软件包。

然后，使用以下cmake命令构建llama.cpp：

```bash
cmake -B build -DGGML_VULKAN=1
cmake --build build --config Release
# 测试输出二进制文件（使用"-ngl 33"将所有层卸载到GPU上）
./bin/llama-cli -m "模型路径" -p "你好，你怎么样" -n 50 -e -ngl 33 -t 4

# 您应该能在输出中看到ggml_vulkan检测到您的GPU。例如：
# ggml_vulkan: 使用英特尔(R)显卡 (ADL GT2) | uma: 1 | fp16: 1 | warp大小: 32
```


### Android

要阅读如何在Android上构建的文档，[点击这里](./android.md)

