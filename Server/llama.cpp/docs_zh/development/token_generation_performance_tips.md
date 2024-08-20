# Token生成性能故障排除

## 验证模型是否使用CUDA在GPU上运行

请确保根据[此指南](../README.md#CUDA)编译了llama，以便llama接受`-ngl N`（或`--n-gpu-layers N`)标志。运行llama时，您可以配置`N`为非常大，llama会将尽可能多的层卸载到GPU上，即使它少于您配置的数量。例如：
```shell
./llama-cli -m "path/to/model.gguf" -ngl 200000 -p "Please sir, may I have some "
```

在运行llama之前，它会输出诊断信息，显示cuBLAS是否将工作卸载到GPU上。寻找这些行：
```shell
llama_model_load_internal: [cublas] offloading 60 layers to GPU
llama_model_load_internal: [cublas] offloading output layer to GPU
llama_model_load_internal: [cublas] total VRAM used: 17223 MB
... rest of inference
```

如果您看到这些行，则说明GPU正在使用。

## 验证CPU是否未过载

Llama接受一个`-t N`（或`--threads N`）参数。这个参数的大小非常重要，它不能太大。如果你的token生成非常慢，请尝试将这个数字设置为1。如果这显著提高了你的token生成速度，那么你的CPU正在过载，你需要显式地将这个参数设置为你的机器上物理CPU核心的数量（即使你使用GPU）。如果你不确定，从1开始，将数量加倍，直到你遇到性能瓶颈，然后减少数量。

# 运行时标志对推理速度基准测试的影响示例

这些运行在以下机器上进行了测试：

GPU：A6000（48GB VRAM）
CPU：7个物理核心
RAM：32GB

模型：`TheBloke_Wizard-Vicuna-30B-Uncensored-GGML/Wizard-Vicuna-30B-Uncensored.q4_0.gguf`（30B参数，4位量化，GGML）

运行命令：`./llama-cli -m "path/to/model.gguf" -p "接下来将是一个极其详细的10种民族菜系的描述，附有食谱：" -n 1000 [额外的基准测试标志]`

结果：

| 命令 | 每秒token数（越高越好） |
| --- | --- |
| -ngl 2000000 | N/A（小于0.1） |
| -t 7 | 1.7 |
| -t 1 -ngl 2000000 | 5.5 |
| -t 7 -ngl 2000000 | 8.7 |
| -t 4 -ngl 2000000 | 9.1 |

