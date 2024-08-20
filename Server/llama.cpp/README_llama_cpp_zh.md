# llama.cpp

![llama](https://user-images.githubusercontent.com/1991296/230134379-7181e485-c521-4d23-a0d6-f7b3b61ba524.png)

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Server](https://github.com/ggerganov/llama.cpp/actions/workflows/server.yml/badge.svg?branch=master&event=schedule)](https://github.com/ggerganov/llama.cpp/actions/workflows/server.yml)
[![Conan Center](https://shields.io/conan/v/llama-cpp)](https://conan.io/center/llama-cpp)

[路线图](https://github.com/users/ggerganov/projects/7) / [项目状态](https://github.com/ggerganov/llama.cpp/discussions/3471) / [宣言](https://github.com/ggerganov/llama.cpp/discussions/205) / [ggml](https://github.com/ggerganov/ggml)

纯C/C++实现Meta的[LLaMA](https://arxiv.org/abs/2302.13971)模型（及其他模型）的推理

> [!IMPORTANT]
[2024年6月12日] 二进制文件已重命名，带有`llama-`前缀。`main`现在是`llama-cli`，`server`是`llama-server`等（https://github.com/ggerganov/llama.cpp/pull/7809）

## 近期API变更

- [2024年6月26日] 重新结构化源代码和CMake构建脚本 https://github.com/ggerganov/llama.cpp/pull/8006
- [2024年4月21日] `llama_token_to_piece` 现在可以可选地渲染特殊令牌 https://github.com/ggerganov/llama.cpp/pull/6807
- [2024年4月4日] 将状态和会话文件函数重新组织到 `llama_state_*` 下 https://github.com/ggerganov/llama.cpp/pull/6341
- [2024年3月26日] 更新Logits和Embeddings API以减小体积 https://github.com/ggerganov/llama.cpp/pull/6122
- [2024年3月13日] 添加 `llama_synchronize()` + `llama_context_params.n_ubatch` https://github.com/ggerganov/llama.cpp/pull/6017
- [2024年3月8日] `llama_kv_cache_seq_rm()` 返回 `bool` 而不是 `void`，并且新的 `llama_n_seq_max()` 返回批处理中可接受的 `seq_id` 的上限（当处理多个序列时相关） https://github.com/ggerganov/llama.cpp/pull/5328
- [2024年3月4日] 更新Embeddings API https://github.com/ggerganov/llama.cpp/pull/5796
- [2024年3月3日] `struct llama_context_params` https://github.com/ggerganov/llama.cpp/pull/5849

## 热门话题

- **`convert.py` 已被弃用，并移动到 `examples/convert_legacy_llama.py`，请使用 `convert_hf_to_gguf.py`** https://github.com/ggerganov/llama.cpp/pull/7430
- 初始 Flash-Attention 支持：https://github.com/ggerganov/llama.cpp/pull/5021
- 已添加 BPE 预标记支持：https://github.com/ggerganov/llama.cpp/pull/6920
- MoE 内存布局已更新 - 重新转换模型以支持 `mmap` 并重新生成 `imatrix` https://github.com/ggerganov/llama.cpp/pull/6387
- 使用 `gguf-split` 的模型分片指令 https://github.com/ggerganov/llama.cpp/discussions/6404
- 修复 Metal 批量推理中的主要错误 https://github.com/ggerganov/llama.cpp/pull/6225
- 支持 Multi-GPU 管道并行 https://github.com/ggerganov/llama.cpp/pull/6017
- 寻找贡献以添加 Deepseek 支持：https://github.com/ggerganov/llama.cpp/issues/5981
- 定量盲测试：https://github.com/ggerganov/llama.cpp/discussions/5962
- 已添加初始 Mamba 支持：https://github.com/ggerganov/llama.cpp/pull/5328

----

## 描述

`llama.cpp`的主要目标是实现LLM推理，设置简单且在多种硬件上（本地和云端）提供最先进的性能。

- 使用纯C/C++实现，无任何依赖
- Apple硅芯片是首选公民 - 通过ARM NEON、Accelerate和Metal框架进行优化
- 支持x86架构的AVX、AVX2和AVX512
- 支持用于快速推理和减少内存使用的1.5位、2位、3位、4位、5位、6位和8位整数量化
- 为在NVIDIA GPU上运行LLM自定义CUDA内核（通过HIP支持AMD GPU）
- 支持Vulkan和SYCL后端
- CPU+GPU混合推理以部分加速超过总VRAM容量的模型

自其[启动](https://github.com/ggerganov/llama.cpp/issues/33#issuecomment-1465108022)以来，该项目得益于许多贡献者的努力，取得了显著改进。它是为[ggml](https://github.com/ggerganov/ggml)库开发新功能的主要实验场所。

**支持的模型：**

通常支持以下基础模型的微调版本。

- [X] LLaMA 🦙
- [x] LLaMA 2 🦙🦙
- [x] LLaMA 3 🦙🦙🦙
- [X] [Mistral 7B](https://huggingface.co/mistralai/Mistral-7B-v0.1)
- [x] [Mixtral MoE](https://huggingface.co/models?search=mistral-ai/Mixtral)
- [x] [DBRX](https://huggingface.co/databricks/dbrx-instruct)
- [X] [Falcon](https://huggingface.co/models?search=tiiuae/falcon)
- [X] [Chinese LLaMA / Alpaca](https://github.com/ymcui/Chinese-LLaMA-Alpaca) 和 [Chinese LLaMA-2 / Alpaca-2](https://github.com/ymcui/Chinese-LLaMA-Alpaca-2)
- [X] [Vigogne (French)](https://github.com/bofenghuang/vigogne)
- [X] [BERT](https://github.com/ggerganov/llama.cpp/pull/5423)
- [X] [Koala](https://bair.berkeley.edu/blog/2023/04/03/koala/)
- [X] [Baichuan 1 & 2](https://huggingface.co/models?search=baichuan-inc/Baichuan) + [衍生](https://huggingface.co/hiyouga/baichuan-7b-sft)
- [X] [Aquila 1 & 2](https://huggingface.co/models?search=BAAI/Aquila)
- [X] [Starcoder models](https://github.com/ggerganov/llama.cpp/pull/3187)
- [X] [Refact](https://huggingface.co/smallcloudai/Refact-1_6B-fim)

- [X] [MPT](https://github.com/ggerganov/llama.cpp/pull/3417)
- [X] [Bloom](https://github.com/ggerganov/llama.cpp/pull/3553)
- [x] [Yi模型](https://huggingface.co/models?search=01-ai/Yi)
- [X] [StableLM模型](https://huggingface.co/stabilityai)
- [x] [Deepseek模型](https://huggingface.co/models?search=deepseek-ai/deepseek)
- [x] [Qwen模型](https://huggingface.co/models?search=Qwen/Qwen)
- [x] [PLaMo-13B](https://github.com/ggerganov/llama.cpp/pull/3557)
- [x] [Phi模型](https://huggingface.co/models?search=microsoft/phi)
- [x] [GPT-2](https://huggingface.co/gpt2)
- [x] [Orion 14B](https://github.com/ggerganov/llama.cpp/pull/5118)
- [x] [InternLM2](https://huggingface.co/models?search=internlm2)
- [x] [CodeShell](https://github.com/WisdomShell/codeshell)
- [x] [Gemma](https://ai.google.dev/gemma)
- [x] [Mamba](https://github.com/state-spaces/mamba)
- [x] [Grok-1](https://huggingface.co/keyfan/grok-1-hf)
- [x] [Xverse](https://huggingface.co/models?search=xverse)
- [x] [Command-R模型](https://huggingface.co/models?search=CohereForAI/c4ai-command-r)
- [x] [SEA-LION](https://huggingface.co/models?search=sea-lion)
- [x] [GritLM-7B](https://huggingface.co/GritLM/GritLM-7B) + [GritLM-8x7B](https://huggingface.co/GritLM/GritLM-8x7B)
- [x] [OLMo](https://allenai.org/olmo)
- [x] [GPT-NeoX](https://github.com/EleutherAI/gpt-neox) + [Pythia](https://github.com/EleutherAI/pythia)

(支持更多模型的说明：[如何添加模型.md](./docs/HOWTO-add-model.md))

**多模态模型：**

- [x] [LLaVA 1.5模型](https://huggingface.co/collections/liuhaotian/llava-15-653aac15d994e992e2677a7e), [LLaVA 1.6模型](https://huggingface.co/collections/liuhaotian/llava-16-65b9e40155f60fd046a5ccf2)
- [x] [BakLLaVA](https://huggingface.co/models?search=SkunkworksAI/Bakllava)
- [x] [Obsidian](https://huggingface.co/NousResearch/Obsidian-3B-V0.5)
- [x] [ShareGPT4V](https://huggingface.co/models?search=Lin-Chen/ShareGPT4V)
- [x] [MobileVLM 1.7B/3B模型](https://huggingface.co/models?search=mobileVLM)
- [x] [Yi-VL](https://huggingface.co/models?search=Yi-VL)
- [x] [Mini CPM](https://huggingface.co/models?search=MiniCPM)

- [x] [Moondream](https://huggingface.co/vikhyatk/moondream2)
- [x] [Bunny](https://github.com/BAAI-DCAI/Bunny)

**绑定：**

- Python: [abetlen/llama-cpp-python](https://github.com/abetlen/llama-cpp-python)
- Go: [go-skynet/go-llama.cpp](https://github.com/go-skynet/go-llama.cpp)
- Node.js: [withcatai/node-llama-cpp](https://github.com/withcatai/node-llama-cpp)
- JS/TS (llama.cpp 服务器客户端): [lgrammel/modelfusion](https://modelfusion.dev/integration/model-provider/llamacpp)
- JavaScript/Wasm (在浏览器中工作): [tangledgroup/llama-cpp-wasm](https://github.com/tangledgroup/llama-cpp-wasm)
- Typescript/Wasm (更友好的API，可在npm上使用): [ngxson/wllama](https://github.com/ngxson/wllama)
- Ruby: [yoshoku/llama_cpp.rb](https://github.com/yoshoku/llama_cpp.rb)
- Rust (更多功能): [edgenai/llama_cpp-rs](https://github.com/edgenai/llama_cpp-rs)
- Rust (更友好的API): [mdrokz/rust-llama.cpp](https://github.com/mdrokz/rust-llama.cpp)
- Rust (更直接的绑定): [utilityai/llama-cpp-rs](https://github.com/utilityai/llama-cpp-rs)
- C#/.NET: [SciSharp/LLamaSharp](https://github.com/SciSharp/LLamaSharp)
- Scala 3: [donderom/llm4s](https://github.com/donderom/llm4s)
- Clojure: [phronmophobic/llama.clj](https://github.com/phronmophobic/llama.clj)
- React Native: [mybigday/llama.rn](https://github.com/mybigday/llama.rn)
- Java: [kherud/java-llama.cpp](https://github.com/kherud/java-llama.cpp)
- Zig: [deins/llama.cpp.zig](https://github.com/Deins/llama.cpp.zig)
- Flutter/Dart: [netdur/llama_cpp_dart](https://github.com/netdur/llama_cpp_dart)
- PHP (基于llama.cpp的API绑定和功能): [distantmagic/resonance](https://github.com/distantmagic/resonance) [(更多信息)](https://github.com/ggerganov/llama.cpp/pull/6326)

**UI:**

除非另有说明，这些项目都采用开放源代码许可：

- [iohub/collama](https://github.com/iohub/coLLaMA)
- [janhq/jan](https://github.com/janhq/jan) (AGPL)
- [nat/openplayground](https://github.com/nat/openplayground)
- [Faraday](https://faraday.dev/) (专有)
- [LMStudio](https://lmstudio.ai/) (专有)
- [Layla](https://play.google.com/store/apps/details?id=com.laylalite) (专有)

- [LocalAI](https://github.com/mudler/LocalAI) (MIT)
- [LostRuins/koboldcpp](https://github.com/LostRuins/koboldcpp) (AGPL)
- [Mozilla-Ocho/llamafile](https://github.com/Mozilla-Ocho/llamafile)
- [nomic-ai/gpt4all](https://github.com/nomic-ai/gpt4all)
- [ollama/ollama](https://github.com/ollama/ollama)
- [oobabooga/text-generation-webui](https://github.com/oobabooga/text-generation-webui) (AGPL)
- [psugihara/FreeChat](https://github.com/psugihara/FreeChat)
- [cztomsik/ava](https://github.com/cztomsik/ava) (MIT)
- [ptsochantaris/emeltal](https://github.com/ptsochantaris/emeltal)
- [pythops/tenere](https://github.com/pythops/tenere) (AGPL)
- [RAGNA 桌面版](https://ragna.app/) (私有)
- [RecurseChat](https://recurse.chat/) (私有)
- [semperai/amica](https://github.com/semperai/amica)
- [withcatai/catai](https://github.com/withcatai/catai)
- [移动人工智能/maid](https://github.com/Mobile-Artificial-Intelligence/maid) (MIT)
- [Msty](https://msty.app) (私有)
- [LLMFarm](https://github.com/guinmoon/LLMFarm?tab=readme-ov-file) (MIT)
- [KanTV](https://github.com/zhouwg/kantv?tab=readme-ov-file)(Apachev2.0或更高版本)
- [Dot](https://github.com/alexpinel/Dot) (GPL)
- [MindMac](https://mindmac.app) (私有)
- [KodiBot](https://github.com/firatkiral/kodibot) (GPL)
- [eva](https://github.com/ylsdamxssjxxdd/eva) (MIT)
- [AI Sublime Text插件](https://github.com/yaroslavyaroslav/OpenAI-sublime-text) (MIT)
- [AIKit](https://github.com/sozercan/aikit) (MIT)
- [LARS - LLM & Advanced Referencing Solution](https://github.com/abgulati/LARS) (AGPL)

*(要在此处列出项目，它应明确声明它依赖于`llama.cpp`)*

**工具：**

- [akx/ggify](https://github.com/akx/ggify) – 从HuggingFace Hub下载PyTorch模型并将其转换为GGML
- [crashr/gppm](https://github.com/crashr/gppm) – 利用NVIDIA Tesla P40或P100 GPU启动llama.cpp实例，以降低空闲功率消耗

**基础设施：**

- [Paddler](https://github.com/distantmagic/paddler) - 定制的有状态负载均衡器，专为llama.cpp设计

## 示例

<details>
<summary>使用LLaMA v2 13B在M2 Ultra上的典型运行</summary>

```bash
$ make -j && ./llama-cli -m models/llama-13b-v2/ggml-model-q4_0.gguf -p "建立网站可以通过10个简单的步骤来完成:\n步骤1:" -n 400 -e
I llama.cpp build info：
I UNAME_S:  Darwin
I UNAME_P:  arm
I UNAME_M:  arm64
I CFLAGS:   -I.            -O3 -std=c11   -fPIC -DNDEBUG -Wall -Wextra -Wpedantic -Wcast-qual -Wdouble-promotion -Wshadow -Wstrict-prototypes -Wpointer-arith -Wmissing-prototypes -pthread -DGGML_USE_K_QUANTS -DGGML_USE_ACCELERATE
I CXXFLAGS: -I. -I./common -O3 -std=c++11 -fPIC -DNDEBUG -Wall -Wextra -Wpedantic -Wcast-qual -Wno-unused-function -Wno-multichar -pthread -DGGML_USE_K_QUANTS
I LDFLAGS:   -framework Accelerate
I CC:       Apple clang version 14.0.3 (clang-1403.0.22.14.1)
I CXX:      Apple clang version 14.0.3 (clang-1403.0.22.14.1)

make: Nothing to be done for `default'。
main: build = 1041 (cf658ad)
main: seed  = 1692823051
llama_model_loader: loaded meta data with 16 key-value pairs and 363 tensors from models/llama-13b-v2/ggml-model-q4_0.gguf (version GGUF V1 (latest))
llama_model_loader: - type  f32:   81 tensors
llama_model_loader: - type q4_0:  281 tensors
llama_model_loader: - type q6_K:    1 tensors
llm_load_print_meta: format         = GGUF V1 (latest)
llm_load_print_meta: arch           = llama
llm_load_print_meta: vocab type     = SPM
llm_load_print_meta: n_vocab        = 32000
llm_load_print_meta: n_merges       = 0
llm_load_print_meta: n_ctx_train    = 4096
llm_load_print_meta: n_ctx          = 512
llm_load_print_meta: n_embd         = 5120
llm_load_print_meta: n_head         = 40
llm_load_print_meta: n_head_kv      = 40
llm_load_print_meta: n_layer        = 40
llm_load_print_meta: n_rot          = 128
llm_load_print_meta: n_gqa          = 1
llm_load_print_meta: f_norm_eps     = 1.0e-05
llm_load_print_meta: f_norm_rms_eps = 1.0e-05
llm_load_print_meta: n_ff           = 13824
llm_load_print_meta: freq_base      = 10000.0
llm_load_print_meta: freq_scale     = 1
llm_load_print_meta: model type     = 13B
llm_load_print_meta: model ftype    = mostly Q4_0
llm_load_print_meta: model size     = 13.02 B
llm_load_print_meta: general.name   = LLaMA v2
llm_load_print_meta: BOS token = 1 '<s>'
llm_load_print_meta: EOS token = 2 '</s>'
llm_load_print_meta: UNK token = 0 '<unk>'
llm_load_print_meta: LF token  = 13 '<0x0A>'
llm_load_tensors: ggml ctx size =    0.11 MB
llm_load_tensors: mem required  = 7024.01 MB (+  400.00 MB per state)
...................................................................................................
llama_new_context_with_model: kv self size  =  400.00 MB
llama_new_context_with_model: compute buffer total size =   75.41 MB

system_info: n_threads = 16 / 24 | AVX = 0 | AVX2 = 0 | AVX512 = 0 | AVX512_VBMI = 0 | AVX512_VNNI = 0 | FMA = 0 | NEON = 1 | ARM_FMA = 1 | F16C = 0 | FP16_VA = 1 | WASM_SIMD = 0 | BLAS = 1 | SSE3 = 0 | VSX = 0 |
sampling: repeat_last_n = 64, repeat_penalty = 1.100000, presence_penalty = 0.000000, frequency_penalty = 0.000000, top_k = 40, tfs_z = 1.000000, top_p = 0.950000, typical_p = 1.000000, temp = 0.800000, mirostat = 0, mirostat_lr = 0.100000, mirostat_ent = 5.000000
generate: n_ctx = 512, n_batch = 512, n_predict = 400, n_keep = 0


建立一个网站可以通过10个简单的步骤完成：
步骤1: 找到合适的网站平台。
步骤2: 选择你的域名和托管计划。
步骤3: 设计你的网站布局。
步骤4: 编写你的网站内容并添加图片。
步骤5: 安装安全功能以保护你的网站免受黑客或垃圾邮件发送者的侵害
步骤6: 在多个浏览器、移动设备、操作系统等上进行网站测试
步骤7: 让与你无关的人再次测试它——朋友或家人就可以！
步骤8: 通过社交媒体渠道或付费广告开始推广和营销网站
步骤9: 分析到目前为止有多少访客访问了你的网站，哪些类型的访客访问更频繁（例如，男性 vs 女性）等...
步骤10: 通过遵循网页设计趋势并保持对新技术的了解，继续改进上述所有方面，以进一步增强用户体验！
网站是如何工作的？
网站通过页面组成，这些页面由HTML代码制成。此代码告诉您的计算机如何显示您访问的每个页面的内容——无论是图像还是文本文件（如PDF）。为了让其他人的浏览器不仅能够，而且愿意在访问任何给定的URL时获得相同的结果；需要通过编程脚本采取一些额外的步骤，这些脚本将添加功能，例如使链接可点击！

最常见的一种类型被称为静态HTML页面，因为它们在时间上保持不变，除非手动修改（直接编辑文件或使用如WordPress这样的界面）。它们通常通过HTTP协议提供服务 - 这意味着任何人都可以访问它们，而无需任何特殊权限，例如成为允许进入在线受限区域的群体的一员；然而，根据一个人居住的地理位置，可能仍然存在一些限制。

如何

llama_print_timings:        加载时间 =   576.45 毫秒
llama_print_timings:      样本时间 =   283.10 毫秒 /   400 次   (    0.71 毫秒每令牌，  1412.91 令牌每秒)
llama_print_timings: prompt eval time =   599.83 毫秒 /    19 令牌 (   31.57 毫秒每令牌，    31.68 令牌每秒)
llama_print_timings:        评估时间 = 24513.59 毫秒 /   399 次   (   61.44 毫秒每令牌，    16.28 令牌每秒)
llama_print_timings:       总时间 = 25431.49 毫秒
```

</details>

<details>
<summary>在单个M1 Pro MacBook上运行LLaMA-7B和whisper.cpp的演示</summary>

以下是另一个在单个M1 Pro MacBook上运行LLaMA-7B和[whisper.cpp](https://github.com/ggerganov/whisper.cpp)的演示：

https://user-images.githubusercontent.com/1991296/224442907-7693d4be-acaa-4e01-8b4f-add84093ffff.mp4

</details>

## 使用方法

以下是大多数支持模型的端到端二进制构建和模型转换步骤。

### 基本使用

首先，你需要获取二进制文件。你可以遵循不同的方法：
- 方法1：克隆此存储库并在本地构建，请参阅[如何构建](./docs/build.md)
- 方法2：如果你使用MacOS或Linux，可以通过[brew, flox或nix](./docs/install.md)安装llama.cpp
- 方法3：使用Docker镜像，请参阅[关于Docker的文档](./docs/docker.md)
- 方法4：从[发布版](https://github.com/ggerganov/llama.cpp/releases)下载预构建的二进制文件

你可以使用以下命令运行一个基本的完成操作：

```bash
llama-cli -m your_model.gguf -p "我相信生命的意义是" -n 128

# 输出：
# 我相信生命的意义是找到你自己的真理，并按照它生活。对我来说，这意味着对自己真诚，追随我的激情，即使它们与社会期望不一致。我认为这就是我喜欢瑜伽的原因——它不仅仅是一种身体练习，也是一种精神练习。它是关于与自己连接，倾听你内心的声音，并尊重你独特的旅程。
```

请参阅[这个页面](./examples/main/README.md)以获取参数的完整列表。

### 对话模式

如果您想要一个更类似ChatGPT的体验，可以通过传递参数`-cnv`来运行对话模式：

```bash
llama-cli -m your_model.gguf -p "You are a helpful assistant" -cnv

# 输出：
# > hi, who are you?
# Hi there! I'm your helpful assistant! I'm an AI-powered chatbot designed to assist and provide information to users like you. I'm here to help answer your questions, provide guidance, and offer support on a wide range of topics. I'm a friendly and knowledgeable AI, and I'm always happy to help with anything you need. What's on your mind, and how can I assist you today?
#
# > what is 1+1?
# Easy peasy! The answer to 1+1 is... 2!
```

默认情况下，聊天模板将从输入模型中获取。如果您想使用另一个聊天模板，可以通过传递参数`--chat-template NAME`来实现。请查看[支持的模板列表](https://github.com/ggerganov/llama.cpp/wiki/Templates-supported-by-llama_chat_apply_template)

```bash
./llama-cli -m your_model.gguf -p "You are a helpful assistant" -cnv --chat-template chatml
```

您还可以通过`in-prefix`、`in-suffix`和`reverse-prompt`参数使用自己的模板：

```bash
./llama-cli -m your_model.gguf -p "You are a helpful assistant" -cnv --in-prefix '用户： ' --reverse-prompt '用户：'
```

### Web服务器

[llama.cpp Web服务器](./examples/server/README.md)是一个轻量级的HTTP服务器，兼容[OpenAI API](https://github.com/openai/openai-openapi)，可以用于提供本地模型，并轻松地将它们连接到现有的客户端。

使用示例：

```bash
./llama-server -m your_model.gguf --port 8080

# 基本Web界面可以通过浏览器访问：http://localhost:8080
# 聊天完成端点：http://localhost:8080/v1/chat/completions
```

### 交互模式

> [!注意]
> 如果您更喜欢基本使用，请考虑使用会话模式而不是交互模式

在这种模式下，您可以通过按Ctrl+C来中断生成，并输入一行或多行文本，这些文本将被转换为令牌并附加到当前上下文中。您还可以使用参数`-r "reverse prompt string"`指定一个*反向提示*。这将在生成过程中遇到与反向提示字符串完全相同的令牌时提示用户输入。典型用途是使用一个提示，使LLaMA模拟Alice和Bob等多人之间的聊天，例如，传递`-r "Alice:"`。

以下是一个几个步骤的交互示例，通过命令调用：

```bash
# 使用7B模型默认参数
./examples/chat.sh

# 使用13B模型的高级聊天
./examples/chat-13B.sh

# 使用13B模型的自定义参数
./llama-cli -m ./models/13B/ggml-model-q4_0.gguf -n 256 --repeat_penalty 1.0 --color -i -r "User:" -f prompts/chat-with-bob.txt
```

注意使用`--color`参数来区分用户输入和生成的文本。其他参数在`llama-cli`示例程序的[README](examples/main/README.md)中有更详细的解释。

![image](https://user-images.githubusercontent.com/1991296/224575029-2af3c7dc-5a65-4f64-a6bb-517a532aea38.png)

### 持久交互

通过使用 `--prompt-cache` 和 `--prompt-cache-all`，`./llama-cli` 调用的提示、用户输入和模型生成可以被保存并重新启动。`./examples/chat-persistent.sh` 脚本展示了如何使用支持长时间运行、可恢复的聊天会话。要使用此示例，您必须提供一个用于缓存初始聊天提示的文件和一个用于保存聊天会话的目录，并且可以可选地提供与 `chat-13B.sh` 相同的变量。相同的提示缓存可以用于新的聊天会话。请注意，提示缓存和聊天目录都与初始提示 (`PROMPT_TEMPLATE`) 和模型文件相关联。

```bash
# 开始一个新的聊天
PROMPT_CACHE_FILE=chat.prompt.bin CHAT_SAVE_DIR=./chat/default ./examples/chat-persistent.sh

# 重新启动那个聊天
PROMPT_CACHE_FILE=chat.prompt.bin CHAT_SAVE_DIR=./chat/default ./examples/chat-persistent.sh

# 使用相同的提示/模型开始另一个聊天
PROMPT_CACHE_FILE=chat.prompt.bin CHAT_SAVE_DIR=./chat/another ./examples/chat-persistent.sh

# 不同提示/模型的不同提示缓存
PROMPT_TEMPLATE=./prompts/chat-with-bob.txt PROMPT_CACHE_FILE=bob.prompt.bin \
    CHAT_SAVE_DIR=./chat/bob ./examples/chat-persistent.sh
```

### 使用语法限制输出

`llama.cpp` 支持语法来限制模型输出。例如，您可以强制模型仅输出JSON：

```bash
./llama-cli -m ./models/13B/ggml-model-q4_0.gguf -n 256 --grammar-file grammars/json.gbnf -p 'Request: schedule a call at 8pm; Command:'
```

`grammars/` 文件夹包含了一些示例语法。要编写自己的语法，请查看[GBNF指南](./grammars/README.md)。

对于编写更复杂的JSON语法，您还可以查看 https://grammar.intrinsiclabs.ai/，这是一个浏览器应用程序，允许您编写TypeScript接口，它将编译成GBNF语法，您可以将它们保存以供本地使用。请注意，该应用程序是由社区成员构建和维护的，请在此仓库[提交任何问题或功能请求](http://github.com/intrinsiclabsai/gbnfgen)。

## 构建

请参阅[本地构建 llama.cpp](./docs/build.md)

## 支持的后端

| 后端 | 目标设备 |
| --- | --- |
| [Metal](./docs/build.md#metal-build) | Apple Silicon |
| [BLAS](./docs/build.md#blas-build) | 所有 |
| [BLIS](./docs/backend/BLIS.md) | 所有 |
| [SYCL](./docs/backend/SYCL.md) | Intel 和 Nvidia GPU |
| [CUDA](./docs/build.md#cuda) | Nvidia GPU |
| [hipBLAS](./docs/build.md#hipblas) | AMD GPU |
| [Vulkan](./docs/build.md#vulkan) | GPU |

## 工具

### 准备和量化

> [!NOTE]
> 您可以使用 Hugging Face 上的 [GGUF-my-repo](https://huggingface.co/spaces/ggml-org/gguf-my-repo) 空间对模型权重进行量化，无需任何设置。它会每6小时从 `llama.cpp` 主文件同步一次。

要获取官方 LLaMA 2 权重，请参阅 <a href="#obtaining-and-using-the-facebook-llama-2-model">获取和使用 Facebook LLaMA 2 模型</a> 部分。Hugging Face 上还提供了大量预量化的 `gguf` 模型。

注意：`convert.py` 已移动到 `examples/convert_legacy_llama.py`，并且仅应用于 `Llama/Llama2/Mistral` 模型及其派生模型。它不支持 LLaMA 3，您可以使用从 Hugging Face 下载的 LLaMA 3 使用 `convert_hf_to_gguf.py`。

要了解更多关于量化模型的信息，[请阅读此文档](./examples/quantize/README.md)

### 难度 (衡量模型质量)

您可以使用 `perplexity` 示例来测量给定提示的难度（较低的难度更好）。
更多信息请参阅 [https://huggingface.co/docs/transformers/perplexity](https://huggingface.co/docs/transformers/perplexity)。

要了解更多如何使用 llama.cpp 测量难度，[请阅读此文档](./examples/perplexity/README.md)

## 贡献

- 贡献者可以打开Pull Request (PR)
- 协作者可以向`llama.cpp`仓库的分支推送更改，并将PR合并到`master`分支
- 协作者将根据其贡献被邀请
- 对于管理和处理问题和PR的任何帮助都非常感激！
- 查看适合首次贡献的任务 [good first issues](https://github.com/ggerganov/llama.cpp/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- 读取[CONTRIBUTING.md](CONTRIBUTING.md)以获取更多信息
- 一定要阅读这个：[边缘推理](https://github.com/ggerganov/llama.cpp/discussions/205)
- 一些背景信息供有兴趣的人参考：[变更日志播客](https://changelog.com/podcast/532)

## 其他文档

- [main (命令行)](./examples/main/README.md)
- [服务器](./examples/server/README.md)
- [Jeopardy](./examples/jeopardy/README.md)
- [GBNF 语法](./grammars/README.md)

**开发文档**

- [如何构建](./docs/build.md)
- [在Docker上运行](./docs/docker.md)
- [在Android上构建](./docs/android.md)
- [性能故障排除](./docs/token_generation_performance_tips.md)
- [GGML技巧与窍门](https://github.com/ggerganov/llama.cpp/wiki/GGML-Tips-&-Tricks)

**关键论文和模型背景**

如果你的问题是关于模型生成质量，那么请至少扫描以下链接和论文，了解LLaMA模型的局限性。这对于选择适当模型大小，并欣赏LLaMA模型与ChatGPT之间的显著和细微差异尤为重要：

- LLaMA：
    - [介绍LLaMA：一个基础性、65亿参数的大语言模型](https://ai.facebook.com/blog/large-language-model-llama-meta-ai/)
    - [LLaMA：开放且高效的基石语言模型](https://arxiv.org/abs/2302.13971)
- GPT-3
    - [语言模型是零样本学习者](https://arxiv.org/abs/2005.14165)
- GPT-3.5 / InstructGPT / ChatGPT：
    - [对齐语言模型以遵循指令](https://openai.com/research/instruction-following)
    - [使用人类反馈训练语言模型以遵循指令](https://arxiv.org/abs/2203.02155)

