# Docker

## 前置条件
* Docker必须已安装并运行在你的系统上。
* 创建一个文件夹来存储大型模型和中间文件（例如 /llama/models）

## 镜像
我们为该项目提供了三个Docker镜像：

1. `ghcr.io/ggerganov/llama.cpp:full`: 这个镜像包含了主可执行文件以及将LLaMA模型转换为ggml和进行4位量化转换的工具。 (平台: `linux/amd64`, `linux/arm64`)
2. `ghcr.io/ggerganov/llama.cpp:light`: 这个镜像只包含了主可执行文件。 (平台: `linux/amd64`, `linux/arm64`)
3. `ghcr.io/ggerganov/llama.cpp:server`: 这个镜像只包含了服务器可执行文件。 (平台: `linux/amd64`, `linux/arm64`)

此外，还有以下与上述镜像类似的镜像：

- `ghcr.io/ggerganov/llama.cpp:full-cuda`: 与`full`相同，但编译时带有CUDA支持。 (平台: `linux/amd64`)
- `ghcr.io/ggerganov/llama.cpp:light-cuda`: 与`light`相同，但编译时带有CUDA支持。 (平台: `linux/amd64`)
- `ghcr.io/ggerganov/llama.cpp:server-cuda`: 与`server`相同，但编译时带有CUDA支持。 (平台: `linux/amd64`)
- `ghcr.io/ggerganov/llama.cpp:full-rocm`: 与`full`相同，但编译时带有ROCm支持。 (平台: `linux/amd64`, `linux/arm64`)
- `ghcr.io/ggerganov/llama.cpp:light-rocm`: 与`light`相同，但编译时带有ROCm支持。 (平台: `linux/amd64`, `linux/arm64`)
- `ghcr.io/ggerganov/llama.cpp:server-rocm`: 与`server`相同，但编译时带有ROCm支持。 (平台: `linux/amd64`, `linux/arm64`)

启用GPU的镜像目前尚未通过CI测试，只是构建了它们。它们没有使用在 [.devops/](.devops/) 中定义的Dockerfiles或 [.github/workflows/docker.yml](.github/workflows/docker.yml) 中定义的GitHub Action定义的任何变化来构建。如果你需要不同的设置（例如，不同的CUDA或ROCm库），你目前需要本地构建镜像。

## 使用

下载模型、将它们转换为ggml并优化它们的最简单方法是使用--all-in-one命令，该命令包含完整的Docker镜像。

将下面的`/path/to/models`替换为实际下载模型的路径。

```bash
docker run -v /path/to/models:/models ghcr.io/ggerganov/llama.cpp:full --all-in-one "/models/" 7B
```

完成之后，您就可以开始玩了！

```bash
docker run -v /path/to/models:/models ghcr.io/ggerganov/llama.cpp:full --run -m /models/7B/ggml-model-q4_0.gguf -p "建立一个网站可以分成10个简单的步骤：" -n 512
```

或者使用轻量级图像：

```bash
docker run -v /path/to/models:/models ghcr.io/ggerganov/llama.cpp:light -m /models/7B/ggml-model-q4_0.gguf -p "建立一个网站可以分成10个简单的步骤：" -n 512
```

或者使用服务器镜像：

```bash
docker run -v /path/to/models:/models -p 8000:8000 ghcr.io/ggerganov/llama.cpp:server -m /models/7B/ggml-model-q4_0.gguf --port 8000 --host 0.0.0.0 -n 512
```

## 使用CUDA的Docker

假设在Linux上已正确安装了[nvidia-container-toolkit](https://github.com/NVIDIA/nvidia-container-toolkit)，或者在云中使用启用了GPU的服务，容器内应该可以访问`cuBLAS`。

## 在本地构建Docker

```bash
docker build -t local/llama.cpp:full-cuda -f .devops/full-cuda.Dockerfile .
docker build -t local/llama.cpp:light-cuda -f .devops/llama-cli-cuda.Dockerfile .
docker build -t local/llama.cpp:server-cuda -f .devops/llama-server-cuda.Dockerfile .
```

根据您的容器主机支持的CUDA环境以及GPU架构，您可能需要传递一些不同的`ARGS`。默认值如下：

- `CUDA_VERSION`设置为`11.7.1`
- `CUDA_DOCKER_ARCH`设置为`all`

生成的镜像基本上与非CUDA镜像相同：

1. `local/llama.cpp:full-cuda`：此镜像包含主可执行文件以及将LLaMA模型转换为ggml和转换为4位量化的工具。
2. `local/llama.cpp:light-cuda`：此镜像只包含主可执行文件。
3. `local/llama.cpp:server-cuda`：此镜像只包含服务器可执行文件。

## 使用方法

在本地构建完成后，使用方法与非CUDA示例类似，但需要添加`--gpus`标志。您还想要使用`--n-gpu-layers`标志。

```bash
docker run --gpus all -v /path/to/models:/models local/llama.cpp:full-cuda --run -m /models/7B/ggml-model-q4_0.gguf -p "构建一个网站只需要10个简单的步骤：" -n 512 --n-gpu-layers 1
docker run --gpus all -v /path/to/models:/models local/llama.cpp:light-cuda -m /models/7B/ggml-model-q4_0.gguf -p "构建一个网站只需要10个简单的步骤：" -n 512 --n-gpu-layers 1
docker run --gpus all -v /path/to/models:/models local/llama.cpp:server-cuda -m /models/7B/ggml-model-q4_0.gguf --port 8000 --host 0.0.0.0 -n 512 --n-gpu-layers 1
```

