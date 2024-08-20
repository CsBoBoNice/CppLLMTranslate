# 将新的模型架构添加到 `llama.cpp`

添加一个模型需要几个步骤：

1. 将模型转换为GGUF格式
2. 在 `llama.cpp` 中定义模型架构
3. 构建GGML图实现

完成这些步骤后，您就可以打开PR了。

另外，检查示例和主要的ggml后端（CUDA、METAL、CPU）是否与新架构兼容也非常重要，特别是：
- [main](../examples/main)
- [imatrix](../examples/imatrix)
- [quantize](../examples/quantize)
- [server](../examples/server)

### 1. 将模型转换为GGUF

这一步在Python中通过使用[gguf](https://pypi.org/project/gguf/)库的`convert`脚本完成。根据模型架构，您可以使用`convert_hf_to_gguf.py`（用于`.pth`格式的`llama/llama2`模型）或`examples/convert_legacy_llama.py`。

转换脚本读取模型配置、分词器、张量名称和数据，并将它们转换为GGUF元数据和张量。

对于HF模型，需要实现以下步骤：

1. 在新的`Model`子类中定义模型的`Model.register`注解，例如：

```python
@Model.register("MyModelForCausalLM")
class MyModel(Model):
    model_arch = gguf.MODEL_ARCH.GROK
```

2. 在[constants.py](../gguf-py/gguf/constants.py)中定义GGUF张量的布局

在`MODEL_ARCH`中添加枚举条目，在`MODEL_ARCH_NAMES`中添加模型的人类友好名称，在`MODEL_TENSORS`中添加GGUF张量名称。

例如，对于`falcon`模型：

```python
    MODEL_ARCH.FALCON: [
        MODEL_TENSOR.TOKEN_EMBD,
        MODEL_TENSOR.OUTPUT_NORM,
        MODEL_TENSOR.OUTPUT,
        MODEL_TENSOR.ATTN_NORM,
        MODEL_TENSOR.ATTN_NORM_2,
        MODEL_TENSOR.ATTN_QKV,
        MODEL_TENSOR.ATTN_OUT,
        MODEL_TENSOR.FFN_DOWN,
        MODEL_TENSOR.FFN_UP,
    ]
```

3. 将原始张量名称映射到GGUF中的标准化等效名称

作为一般规则，在将新的张量名称添加到GGUF之前，请确保等效的命名已经不存在。

一旦找到GGUF张量名称的等效名称，将其添加到[tensor_mapping.py](../gguf-py/gguf/tensor_mapping.py)文件中。

如果张量名称是重复层/块的组成部分，则关键字 `bid` 替代它。

以注意力层中的归一化张量为例：

```python
block_mappings_cfg: dict[MODEL_TENSOR, tuple[str, ...]] = {
        # 注意力归一化
        MODEL_TENSOR.ATTN_NORM: (
            "gpt_neox.layers.{bid}.input_layernorm",                # gptneox
            "transformer.h.{bid}.ln_1",                             # gpt2 gpt-j refact qwen
            "transformer.blocks.{bid}.norm_1",                      # mpt
            ...
        )
}
```

`transformer.blocks.{bid}.norm_1` 将在 GGUF 中映射到 `blk.{bid}.attn_norm`。

根据模型配置、分词器、代码和张量布局，您可能需要重写：
- `Model#set_gguf_parameters`
- `Model#set_vocab`
- `Model#write_tensors`

注意：张量名称必须以 `.weight` 后缀结尾，这是约定，并且几个工具如 `quantize` 预期这是处理权重的先决条件。

### 2. 在 `llama.cpp` 中定义模型架构

模型参数和张量布局必须在 `llama.cpp` 中定义：
1. 定义一个新的 `llm_arch`
2. 在 `LLM_TENSOR_NAMES` 中定义张量布局
3. 在 `llm_load_hparams` 中添加任何非标准元数据
4. 在 `llm_load_tensors` 中创建推理的张量
5. 如果模型具有 RoPE 操作，则在 `llama_rope_type` 中添加 rope 类型

注意：`ggml` 中的维度通常与 `pytorch` 维度的顺序相反。

### 3. 构建GGML图实现

这是最有意思的部分，你需要在`llama_build_graph`中提供新模型架构的推理图实现。

查看现有实现，如`build_llama`、`build_dbrx`或`build_bert`。

在实现新图时，请注意，底层的`ggml`后端可能不支持所有操作，对缺失后端操作的支撑可以在另一个PR中添加。

注意：要调试推理图，可以使用[llama-eval-callback](../examples/eval-callback)。

## GGUF规范

https://github.com/ggerganov/ggml/blob/master/docs/gguf.md

## 资源

- YaRN RoPE缩放 https://github.com/ggerganov/llama.cpp/pull/2268
- 支持百川序列模型 https://github.com/ggerganov/llama.cpp/pull/3009
- 支持注意力偏差 https://github.com/ggerganov/llama.cpp/pull/4283
- Mixtral支持 https://github.com/ggerganov/llama.cpp/pull/4406
- BERT嵌入 https://github.com/ggerganov/llama.cpp/pull/5423
- Grok-1支持 https://github.com/ggerganov/llama.cpp/pull/6204
- Command R Plus支持 https://github.com/ggerganov/llama.cpp/pull/6491
- 支持架构DBRX https://github.com/ggerganov/llama.cpp/pull/6515
- 如何将HuggingFace模型转换为GGUF格式 https://github.com/ggerganov/llama.cpp/discussions/2948

