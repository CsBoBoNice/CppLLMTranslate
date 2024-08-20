# 量化

您还可以使用Hugging Face上的[GGUF-my-repo](https://huggingface.co/spaces/ggml-org/gguf-my-repo)空间来构建自己的量化模型，无需任何设置。

注意：它每6小时从llama.cpp的`main`函数中同步。

使用示例：

```bash
# 获取官方LLaMA模型权重并将它们放置在./models目录下
ls ./models
llama-2-7b tokenizer_checklist.chk tokenizer.model
# [可选] 对于使用BPE标记器的模型
ls ./models
<文件夹包含权重和标记器json文件> vocab.json
# [可选] 对于PyTorch .bin模型如Mistral-7B
ls ./models
<文件夹包含权重和标记器json文件>

# 安装Python依赖
python3 -m pip install -r requirements.txt

# 将模型转换为ggml FP16格式
python3 convert_hf_to_gguf.py models/mymodel/

# 将模型量化到4位（使用Q4_K_M方法）
./llama-quantize ./models/mymodel/ggml-model-f16.gguf ./models/mymodel/ggml-model-Q4_K_M.gguf Q4_K_M

# 如果旧版本不再受支持，则更新gguf文件类型到当前版本
./llama-quantize ./models/mymodel/ggml-model-Q4_K_M.gguf ./models/mymodel/ggml-model-Q4_K_M-v2.gguf COPY

运行量化模型：

```bash
# 在gguf模型上启动推理
./llama-cli -m ./models/mymodel/ggml-model-Q4_K_M.gguf -n 128
```

在运行较大模型时，请确保您有足够的磁盘空间来存储所有中间文件。

## 内存/磁盘需求

由于模型目前完全加载到内存中，您将需要足够的磁盘空间来保存它们，以及足够的RAM来加载它们。目前，内存和磁盘需求是相同的。

| 模型 | 原始大小 | 量化大小（Q4_0） |
|-----:|---------:|----------------:|
|   7B |        13 GB |              3.9 GB |
|  13B |        24 GB |              7.8 GB |
|  30B |        60 GB |             19.5 GB |
|  65B |       120 GB |             38.5 GB |

## 量化

支持多种量化方法。它们在结果模型的磁盘大小和推理速度上有所不同。

*(已过时)*

| 模型 | 测量指标 |    F16 |   Q4_0 |   Q4_1 |   Q5_0 |   Q5_1 |   Q8_0 |
|-----:|---------:|-------:|-------:|-------:|-------:|-------:|-------:|
|   7B | 混淆度   | 5.9066 | 6.1565 | 6.0912 | 5.9862 | 5.9481 | 5.9070 |
|   7B | 文件大小 |  13.0G |   3.5G |   3.9G |   4.3G |   4.7G |   6.7G |
|   7B | 每tok毫秒 |    127 |     55 |     54 |     76 |     83 |     72 |
|   7B | 每tok毫秒 |    122 |     43 |     45 |     52 |     56 |     67 |
|   7B | 每权重位 |   16.0 |    4.5 |    5.0 |    5.5 |    6.0 |    8.5 |
|  13B | 混淆度   | 5.2543 | 5.3860 | 5.3608 | 5.2856 | 5.2706 | 5.2548 |
|  13B | 文件大小 |  25.0G |   6.8G |   7.6G |   8.3G |   9.1G |    13G |
|  13B | 每tok毫秒 |      - |    103 |    105 |    148 |    160 |    131 |
|  13B | 每tok毫秒 |      - |     73 |     82 |    98 |    105 |    128 |
|  13B | 每权重位 |   16.0 |    4.5 |    5.0 |    5.5 |    6.0 |    8.5 |

- [k-量化](https://github.com/ggerganov/llama.cpp/pull/1684)
- 最近k-量化改进和新i-量化
  - [#2707](https://github.com/ggerganov/llama.cpp/pull/2707)
  - [#2807](https://github.com/ggerganov/llama.cpp/pull/2807)
  - [#4773 - 2位i-量化（推理）](https://github.com/ggerganov/llama.cpp/pull/4773)
  - [#4856 - 2位i-量化（推理）](https://github.com/ggerganov/llama.cpp/pull/4856)
  - [#4861 - 重要性矩阵](https://github.com/ggerganov/llama.cpp/pull/4861)
  - [#4872 - MoE 模型](https://github.com/ggerganov/llama.cpp/pull/4872)

- [#4897 - 2位量化](https://github.com/ggerganov/llama.cpp/pull/4897)
- [#4930 - 所有k量化的imatrix](https://github.com/ggerganov/llama.cpp/pull/4930)
- [#4951 - GPU上的imatrix](https://github.com/ggerganov/llama.cpp/pull/4957)
- [#4969 - 旧量化的imatrix](https://github.com/ggerganov/llama.cpp/pull/4969)
- [#4996 - k量化的调整](https://github.com/ggerganov/llama.cpp/pull/4996)
- [#5060 - Q3_K_XS](https://github.com/ggerganov/llama.cpp/pull/5060)
- [#5196 - 3位i量化](https://github.com/ggerganov/llama.cpp/pull/5196)
- [量化调整](https://github.com/ggerganov/llama.cpp/pull/5320), [另一个](https://github.com/ggerganov/llama.cpp/pull/5334), 和 [另一个](https://github.com/ggerganov/llama.cpp/pull/5361)

**Llama 2 7B**

| 量化 | 每权重比特数 (BPW) |
|-----|-------------------|
| Q2_K | 3.35              |
| Q3_K_S | 3.50              |
| Q3_K_M | 3.91              |
| Q3_K_L | 4.27              |
| Q4_K_S | 4.58              |
| Q4_K_M | 4.84              |
| Q5_K_S | 5.52              |
| Q5_K_M | 5.68              |
| Q6_K | 6.56              |

**Llama 2 13B**

量化 | 每权重比特数 (BPW)
-- | ------------------
Q2_K | 3.34
Q3_K_S | 3.48
Q3_K_M | 3.89
Q3_K_L | 4.26
Q4_K_S | 4.56
Q4_K_M | 4.83
Q5_K_S | 5.51
Q5_K_M | 5.67
Q6_K | 6.56

**Llama 2 70B**

量化 | 每权重比特数 (BPW)
-- | ------------------
Q2_K | 3.40
Q3_K_S | 3.47
Q3_K_M | 3.85
Q3_K_L | 4.19
Q4_K_S | 4.53
Q4_K_M | 4.80
Q5_K_S | 5.50
Q5_K_M | 5.65
Q6_K | 6.56

