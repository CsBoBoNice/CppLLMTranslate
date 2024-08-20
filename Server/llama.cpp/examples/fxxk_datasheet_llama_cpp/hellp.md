




```

使用方法：.\build\bin\Release\llama-cli.exe [选项]
通用选项：
  -h,    --help, --usage          打印使用方法并退出
         --version                显示版本和构建信息
  -v,    --verbose                打印详细信息
         --verbosity N            设置特定的详细级别（默认：0）
         --verbose-prompt         在生成前打印详细提示（默认：false）
         --no-display-prompt      在生成时不打印提示（默认：false）
  -co,   --color                  为输出着色，以区分提示和用户输入与生成内容（默认：false）
  -s,    --seed SEED              随机数生成器种子（默认：-1，小于0时使用随机种子）
  -t,    --threads N              生成过程中使用的线程数（默认：8）
  -tb,   --threads-batch N        批处理和提示处理过程中使用的线程数（默认：与--threads相同）
  -td,   --threads-draft N        生成过程中使用的线程数（默认：与--threads相同）
  -tbd,  --threads-batch-draft N  批处理和提示处理过程中使用的线程数（默认：与--threads-draft相同）
         --draft N                为推测解码起草的标记数（默认：5）
  -ps,   --p-split N              推测解码分裂概率（默认：0.1）
  -lcs,  --lookup-cache-static FNAME
                                  静态查找缓存的路径，用于查找解码（生成时不会更新）
  -lcd,  --lookup-cache-dynamic FNAME
                                  动态查找缓存的路径，用于查找解码（生成时会更新）
  -c,    --ctx-size N             提示上下文的大小（默认：0，0 = 从模型加载）
  -n,    --predict N              预测的标记数（默认：-1，-1 = 无限，-2 = 直到上下文填满）
  -b,    --batch-size N           逻辑最大批处理大小（默认：2048）
  -ub,   --ubatch-size N          物理最大批处理大小（默认：512）
         --keep N                 从初始提示中保留的标记数（默认：0，-1 = 所有）
         --chunks N               要处理的最大块数（默认：-1，-1 = 所有）
  -fa,   --flash-attn             启用Flash Attention（默认：禁用）
  -p,    --prompt PROMPT          开始生成的提示
                                  在对话模式下，这将被用作系统提示
                                  （默认：''）
  -f,    --file FNAME             包含提示的文件（默认：无）
         --in-file FNAME          输入文件（重复指定多个文件）
  -bf,   --binary-file FNAME      包含提示的二进制文件（默认：无）
  -e,    --escape                 处理转义序列（\n, \r, \t, \', \", \\）（默认：true）
         --no-escape              不处理转义序列
  -ptc,  --print-token-count N    每隔N个标记打印一次标记数（默认：-1）
         --prompt-cache FNAME     用于缓存提示状态的文件，以便更快启动（默认：无）
         --prompt-cache-all       如果指定，将用户输入和生成内容保存到缓存中
                                  不支持与--interactive或其他交互式选项一起使用
         --prompt-cache-ro        如果指定，使用提示缓存但不更新它
  -r,    --reverse-prompt PROMPT  在PROMPT处停止生成，在交互模式下返回控制
                                  可以多次指定以设置多个提示
  -sp,   --special                启用特殊标记输出（默认：false）
  -cnv,  --conversation           在对话模式下运行，不打印特殊标记和后缀/前缀
                                  如果未指定后缀/前缀，将使用默认聊天模板
                                  （默认：false）
  -i,    --interactive            在交互模式下运行（默认：false）
  -if,   --interactive-first      在交互模式下运行，并立即等待输入（默认：false）
  -mli,  --multiline-input        允许您写入或粘贴多行，而不必每行都以'\'结尾
         --in-prefix-bos          在用户输入前加上BOS前缀，位于`--in-prefix`字符串之前
         --in-prefix STRING       用于前缀用户输入的字符串（默认：空）
         --in-suffix STRING       用于后缀用户输入的字符串（默认：空）
        
采样：
         --samplers SAMPLERS      将按顺序使用的采样器，用';'分隔（默认：top_k;tfs_z;typical_p;top_p;min_p;temperature）
         --sampling-seq SEQUENCE  将使用的采样器的简化序列（默认：kfypmt）
         --ignore-eos             忽略流结束标记并继续生成（隐含参数：--logit-bias EOS-inf）
         --penalize-nl            对换行符进行惩罚（默认：false）
         --temp N                 温度（默认：0.8）
         --top-k N                top-k采样（默认：40，0 = 禁用）
         --top-p N                top-p采样（默认：0.9，1.0 = 禁用）
         --min-p N                min-p采样（默认：0.1，0.0 = 禁用）
         --tfs N                  尾部自由采样，参数z（默认：1.0，1.0 = 禁用）
         --typical N              局部典型采样，参数p（默认：1.0，1.0 = 禁用）
         --repeat-last-n N        考虑进行惩罚的最后n个标记（默认：64，0 = 禁用，-1 = ctx_size）
         --repeat-penalty N       对重复标记序列进行惩罚（默认：1.0，1.0 = 禁用）
         --presence-penalty N     重复alpha存在惩罚（默认：0.0，0.0 = 禁用）
         --frequency-penalty N    重复alpha频率惩罚（默认：0.0，0.0 = 禁用）
         --dynatemp-range N       动态温度范围（默认：0.0，0.0 = 禁用）
         --dynatemp-exp N         动态温度指数（默认：1.0）
         --mirostat N             使用Mirostat采样。
                                  如果使用，将忽略Top K，Nucleus，Tail Free和Locally Typical采样器。
                                  （默认：0，0 = 禁用，1 = Mirostat，2 = Mirostat 2.0）
         --mirostat-lr N          Mirostat学习率，参数eta（默认：0.1）
         --mirostat-ent N         Mirostat目标熵，参数tau（默认：5.0）
         -l TOKEN_ID(+/-)BIAS     修改标记在补全中出现的可能性，
                                  例如 `--logit-bias 15043+1` 增加标记 ' Hello' 出现的可能性，
                                  或 `--logit-bias 15043-1` 减少标记 ' Hello' 出现的可能性
         --cfg-negative-prompt PROMPT
                                  用于指导的负面提示（默认：''）
         --cfg-negative-prompt-file FNAME
                                  用于指导的负面提示文件
         --cfg-scale N            指导的强度（默认：1.0，1.0 = 禁用）
         --chat-template JINJA_TEMPLATE
                                  设置自定义的jinja聊天模板（默认：从模型的元数据中获取模板）
                                  如果指定了后缀/前缀，模板将被禁用
                                  只接受常用的模板：
                                  https://github.com/ggerganov/llama.cpp/wiki/Templates-supported-by-llama_chat_apply_template

语法：
         --grammar GRAMMAR        用于限制生成的BNF类语法（请参考grammars/目录中的示例）(默认: '')
         --grammar-file FNAME     从该文件读取语法
  -j,    --json-schema SCHEMA     用于限制生成的JSON架构（https://json-schema.org/），例如，`{}`代表任意JSON对象
                                  对于包含外部$refs的架构，请使用--grammar + example/json_schema_to_grammar.py
嵌入：
         --pooling {none,mean,cls,last}
                                  嵌入的池化类型，如果未指定则使用模型默认值
         --attention {causal,non-causal}
                                  嵌入的注意力类型，如果未指定则使用模型默认值
上下文破解：
         --rope-scaling {none,linear,yarn}
                                  RoPE频率缩放方法，默认为线性，除非模型指定
         --rope-scale N           RoPE上下文缩放因子，通过N倍扩展上下文
         --rope-freq-base N       RoPE基础频率，用于NTK感知缩放（默认：从模型加载）
         --rope-freq-scale N      RoPE频率缩放因子，通过1/N倍扩展上下文
         --yarn-orig-ctx N        YaRN：模型的原始上下文大小（默认：0 = 模型训练上下文大小）
         --yarn-ext-factor N      YaRN：外推混合因子（默认：-1.0，0.0 = 完全内插）
         --yarn-attn-factor N     YaRN：缩放sqrt(t)或注意力大小（默认：1.0）
         --yarn-beta-slow N       YaRN：高修正维度或alpha（默认：1.0）
         --yarn-beta-fast N       YaRN：低修正维度或beta（默认：32.0）
  -gan,  --grp-attn-n N           组注意力因子（默认：1）
  -gaw,  --grp-attn-w N           组注意力宽度（默认：512.0）
  -dkvc, --dump-kv-cache          KV缓存详细打印
  -nkvo, --no-kv-offload          禁用KV卸载
  -ctk,  --cache-type-k TYPE      KV缓存数据类型对于K（默认：f16）
  -ctv,  --cache-type-v TYPE      KV缓存数据类型对于V（默认：f16）
困惑度：
         --all-logits             为批次中的所有令牌返回logits（默认：false）
         --hellaswag              使用-f提供的datafile中的随机任务计算HellaSwag分数
         --hellaswag-tasks N      计算HellaSwag分数时使用的任务数量（默认：400）
         --winogrande             使用-f提供的datafile中的随机任务计算Winogrande分数
         --winogrande-tasks N     计算Winogrande分数时使用的任务数量（默认：0）
         --multiple-choice        使用-f提供的datafile中的随机任务计算多项选择分数
         --multiple-choice-tasks N
                                  计算多项选择分数时使用的任务数量（默认：0）
         --kl-divergence          计算到通过--kl-divergence-base提供的logits的KL散度
         --ppl-stride N           困惑度计算的步长（默认：0）
         --ppl-output-type {0,1}  困惑度计算输出类型（默认：0）
并行：
  -dt,   --defrag-thold N         KV缓存碎片整理阈值（默认：-1.0，< 0 - 禁用）
  -np,   --parallel N             要解码的并行序列数（默认：1）
  -ns,   --sequences N            要解码的序列数（默认：1）
  -cb,   --cont-batching          启用连续批处理（又名动态批处理）(默认：启用)

多模态：
         --mmproj FILE            指定LLaVA的多模态投影文件路径。详情请查看 examples/llava/README.md
         --image FILE             指定图像文件路径。与多模态模型一起使用。可多次指定以进行批处理
后端：
         --rpc SERVERS            以逗号分隔的RPC服务器列表
         --mlock                  强制系统将模型保留在RAM中，而不是进行交换或压缩
         --no-mmap                不对模型进行内存映射（加载较慢，但如果未使用mlock，可能减少页面输出）
         --numa TYPE              在某些NUMA系统上尝试优化
                                    - distribute: 在所有节点上均匀分布执行
                                    - isolate: 仅在启动执行的节点上的CPU上创建线程
                                    - numactl: 使用numactl提供的CPU映射
                                  如果之前未运行此选项，建议在使用此选项前清空系统页面缓存
                                  请查看 https://github.com/ggerganov/llama.cpp/issues/1437
  -ngl,  --gpu-layers N           存储在VRAM中的层数
  -ngld, --gpu-layers-draft N     为草稿模型存储在VRAM中的层数
  -sm,   --split-mode SPLIT_MODE  如何在多个GPU上分割模型，选项有：
                                    - none: 仅使用一个GPU
                                    - layer (默认): 在GPU间分割层和KV
                                    - row: 在GPU间分割行
  -ts,   --tensor-split SPLIT     将模型的某部分卸载到每个GPU的比例，以逗号分隔的列表形式，例如 3,1
  -mg,   --main-gpu i             用于模型的GPU（当split-mode = none时），
                                  或用于中间结果和KV的GPU（当split-mode = row时）（默认：0）
模型：
         --check-tensors          检查模型张量数据是否含有无效值（默认：false）
         --override-kv KEY=TYPE:VALUE
                                  高级选项，通过键覆盖模型元数据。可多次指定。
                                  类型：int, float, bool, str。例如：--override-kv tokenizer.ggml.add_bos_token=bool:false
         --lora FNAME             应用LoRA适配器（隐含 --no-mmap）
         --lora-scaled FNAME S    应用带有用户定义缩放S的LoRA适配器（隐含 --no-mmap）
         --lora-base FNAME        可选的模型，用作LoRA适配器修改层的基模型
         --control-vector FNAME   添加一个控制向量
                                  注意：此参数可重复多次以添加多个控制向量
         --control-vector-scaled FNAME SCALE
                                  添加一个带有用户定义缩放SCALE的控制向量
                                  注意：此参数可重复多次以添加多个缩放控制向量
         --control-vector-layer-range START END
                                  应用控制向量的层范围，包括起始和结束层
  -m,    --model FNAME            模型路径（默认：models/$filename，其中filename来自 --hf-file
                                  或 --model-url 如果设置，否则 models/7B/ggml-model-f16.gguf）
  -md,   --model-draft FNAME      投机解码的草稿模型（默认：未使用）
  -mu,   --model-url MODEL_URL    模型下载url（默认：未使用）
  -hfr,  --hf-repo REPO           Hugging Face模型仓库（默认：未使用）
  -hff,  --hf-file FILE           Hugging Face模型文件（默认：未使用）
  -hft,  --hf-token TOKEN         Hugging Face访问令牌（默认：来自HF_TOKEN环境变量的值）
检索：
         --context-file FNAME     加载上下文的文件（重复指定多个文件）
         --chunk-size N           嵌入文本块的最小长度（默认：64）
         --chunk-separator STRING
                                  块之间的分隔符（默认：'
                                  ')
passkey：
         --junk N                 重复垃圾文本的次数（默认：250）
         --pos N                  垃圾文本中passkey的位置（默认：-1）
imatrix：
  -o,    --output FNAME           输出文件（默认：'imatrix.dat'）
         --output-frequency N     每N次迭代输出一次imatrix（默认：10）
         --save-frequency N       每N次迭代保存一次imatrix副本（默认：0）
         --process-output         收集输出张量的数据（默认：false）
         --no-ppl                 不计算困惑度（默认：true）
         --
服务器：
         --host HOST              监听的IP地址（默认：127.0.0.1）
         --port PORT              监听的端口（默认：8080）
         --path PATH              提供静态文件的路径（默认：空）
         --embedding(s)           启用embedding端点（默认：禁用）
         --api-key KEY            用于身份验证的API密钥（默认：无）
         --api-key-file FNAME     包含API密钥的文件路径（默认：无）
         --ssl-key-file FNAME     PEM编码的SSL私钥文件路径
         --ssl-cert-file FNAME    PEM编码的SSL证书文件路径
         --timeout N              服务器读写超时时间（以秒为单位，默认：600）
         --threads-http N         处理HTTP请求的线程数（默认：-1）
         --system-prompt-file FNAME
                                  设置一个文件以加载系统提示（所有槽位的初始提示），这对于聊天应用程序很有用
         --log-format {text,json}
                                  日志输出格式：json或text（默认：json）
         --metrics                启用与prometheus兼容的度量端点（默认：禁用）
         --no-slots               禁用槽位监控端点（默认：启用）
         --slot-save-path PATH    保存槽位kv缓存的路径（默认：禁用）
         --chat-template JINJA_TEMPLATE
                                  设置自定义的jinja聊天模板（默认：从模型的元数据中获取模板）
                                  只接受常用的模板：
                                  https://github.com/ggerganov/llama.cpp/wiki/Templates-supported-by-llama_chat_apply_template
  -sps,  --slot-prompt-similarity SIMILARITY
                                  请求的提示与槽位提示匹配的程度，以使用该槽位（默认：0.50，0.0 = 禁用）
日志记录：
         --simple-io              在子进程和有限的控制台中，使用基本的IO以获得更好的兼容性
  -ld,   --logdir LOGDIR          在此路径下保存YAML日志（如果未设置，则不记录日志）
         --log-test               运行简单的日志记录测试
         --log-disable            禁用跟踪日志
         --log-enable             启用跟踪日志
         --log-file FNAME         指定一个日志文件名（不带扩展名）
         --log-new                在启动时创建一个单独的新日志文件。每个日志文件将有一个唯一的名称："\<name\>.\<ID\>.log"
         --log-append             不要截断旧的日志文件。
cvector：
  -o,    --output FNAME           输出文件（默认：'control_vector.gguf'）
         --positive-file FNAME    正面提示文件，每行一个提示（默认：'examples/cvector-generator/positive.txt'）
         --negative-file FNAME    负面提示文件，每行一个提示（默认：'examples/cvector-generator/negative.txt'）
         --pca-batch N            用于PCA的批处理大小。更大的批次运行得更快，但使用更多的内存（默认：100）
         --pca-iter N             用于PCA的迭代次数（默认：1000）
         --method {pca,mean}      要使用的降维方法（默认：pca）


```

```
usage: .\build\bin\Release\llama-cli.exe [options]

general:

  -h,    --help, --usage          print usage and exit
         --version                show version and build info
  -v,    --verbose                print verbose information
         --verbosity N            set specific verbosity level (default: 0)
         --verbose-prompt         print a verbose prompt before generation (default: false)
         --no-display-prompt      don't print prompt at generation (default: false)
  -co,   --color                  colorise output to distinguish prompt and user input from generations (default: false)
  -s,    --seed SEED              RNG seed (default: -1, use random seed for < 0)
  -t,    --threads N              number of threads to use during generation (default: 8)
  -tb,   --threads-batch N        number of threads to use during batch and prompt processing (default: same as --threads)
  -td,   --threads-draft N        number of threads to use during generation (default: same as --threads)
  -tbd,  --threads-batch-draft N  number of threads to use during batch and prompt processing (default: same as --threads-draft)
         --draft N                number of tokens to draft for speculative decoding (default: 5)
  -ps,   --p-split N              speculative decoding split probability (default: 0.1)
  -lcs,  --lookup-cache-static FNAME
                                  path to static lookup cache to use for lookup decoding (not updated by generation)
  -lcd,  --lookup-cache-dynamic FNAME
                                  path to dynamic lookup cache to use for lookup decoding (updated by generation)
  -c,    --ctx-size N             size of the prompt context (default: 0, 0 = loaded from model)
  -n,    --predict N              number of tokens to predict (default: -1, -1 = infinity, -2 = until context filled)
  -b,    --batch-size N           logical maximum batch size (default: 2048)
  -ub,   --ubatch-size N          physical maximum batch size (default: 512)
         --keep N                 number of tokens to keep from the initial prompt (default: 0, -1 = all)
         --chunks N               max number of chunks to process (default: -1, -1 = all)
  -fa,   --flash-attn             enable Flash Attention (default: disabled)
  -p,    --prompt PROMPT          prompt to start generation with
                                  in conversation mode, this will be used as system prompt
                                  (default: '')
  -f,    --file FNAME             a file containing the prompt (default: none)
         --in-file FNAME          an input file (repeat to specify multiple files)
  -bf,   --binary-file FNAME      binary file containing the prompt (default: none)
  -e,    --escape                 process escapes sequences (\n, \r, \t, \', \", \\) (default: true)
         --no-escape              do not process escape sequences
  -ptc,  --print-token-count N    print token count every N tokens (default: -1)
         --prompt-cache FNAME     file to cache prompt state for faster startup (default: none)
         --prompt-cache-all       if specified, saves user input and generations to cache as well
                                  not supported with --interactive or other interactive options
         --prompt-cache-ro        if specified, uses the prompt cache but does not update it
  -r,    --reverse-prompt PROMPT  halt generation at PROMPT, return control in interactive mode
                                  can be specified more than once for multiple prompts
  -sp,   --special                special tokens output enabled (default: false)
  -cnv,  --conversation           run in conversation mode, does not print special tokens and suffix/prefix
                                  if suffix/prefix are not specified, default chat template will be used
                                  (default: false)
  -i,    --interactive            run in interactive mode (default: false)
  -if,   --interactive-first      run in interactive mode and wait for input right away (default: false)
  -mli,  --multiline-input        allows you to write or paste multiple lines without ending each in '\'
         --in-prefix-bos          prefix BOS to user inputs, preceding the `--in-prefix` string
         --in-prefix STRING       string to prefix user inputs with (default: empty)
         --in-suffix STRING       string to suffix after user inputs with (default: empty)
         --spm-infill             use Suffix/Prefix/Middle pattern for infill (instead of Prefix/Suffix/Middle) as some models prefer this. (default: disabled)

sampling:

         --samplers SAMPLERS      samplers that will be used for generation in the order, separated by ';'
                                  (default: top_k;tfs_z;typical_p;top_p;min_p;temperature)
         --sampling-seq SEQUENCE  simplified sequence for samplers that will be used (default: kfypmt)
         --ignore-eos             ignore end of stream token and continue generating (implies --logit-bias EOS-inf)
         --penalize-nl            penalize newline tokens (default: false)
         --temp N                 temperature (default: 0.8)
         --top-k N                top-k sampling (default: 40, 0 = disabled)
         --top-p N                top-p sampling (default: 0.9, 1.0 = disabled)
         --min-p N                min-p sampling (default: 0.1, 0.0 = disabled)
         --tfs N                  tail free sampling, parameter z (default: 1.0, 1.0 = disabled)
         --typical N              locally typical sampling, parameter p (default: 1.0, 1.0 = disabled)
         --repeat-last-n N        last n tokens to consider for penalize (default: 64, 0 = disabled, -1 = ctx_size)
         --repeat-penalty N       penalize repeat sequence of tokens (default: 1.0, 1.0 = disabled)
         --presence-penalty N     repeat alpha presence penalty (default: 0.0, 0.0 = disabled)
         --frequency-penalty N    repeat alpha frequency penalty (default: 0.0, 0.0 = disabled)
         --dynatemp-range N       dynamic temperature range (default: 0.0, 0.0 = disabled)
         --dynatemp-exp N         dynamic temperature exponent (default: 1.0)
         --mirostat N             use Mirostat sampling.
                                  Top K, Nucleus, Tail Free and Locally Typical samplers are ignored if used.
                                  (default: 0, 0 = disabled, 1 = Mirostat, 2 = Mirostat 2.0)
         --mirostat-lr N          Mirostat learning rate, parameter eta (default: 0.1)
         --mirostat-ent N         Mirostat target entropy, parameter tau (default: 5.0)
         -l TOKEN_ID(+/-)BIAS     modifies the likelihood of token appearing in the completion,
                                  i.e. `--logit-bias 15043+1` to increase likelihood of token ' Hello',
                                  or `--logit-bias 15043-1` to decrease likelihood of token ' Hello'
         --cfg-negative-prompt PROMPT
                                  negative prompt to use for guidance (default: '')
         --cfg-negative-prompt-file FNAME
                                  negative prompt file to use for guidance
         --cfg-scale N            strength of guidance (default: 1.0, 1.0 = disable)
         --chat-template JINJA_TEMPLATE
                                  set custom jinja chat template (default: template taken from model's metadata)
                                  if suffix/prefix are specified, template will be disabled
                                  only commonly used templates are accepted:
                                  https://github.com/ggerganov/llama.cpp/wiki/Templates-supported-by-llama_chat_apply_template

grammar:

         --grammar GRAMMAR        BNF-like grammar to constrain generations (see samples in grammars/ dir) (default: '')
         --grammar-file FNAME     file to read grammar from
  -j,    --json-schema SCHEMA     JSON schema to constrain generations (https://json-schema.org/), e.g. `{}` for any JSON object
                                  For schemas w/ external $refs, use --grammar + example/json_schema_to_grammar.py instead

embedding:

         --pooling {none,mean,cls,last}
                                  pooling type for embeddings, use model default if unspecified
         --attention {causal,non-causal}
                                  attention type for embeddings, use model default if unspecified

context hacking:

         --rope-scaling {none,linear,yarn}
                                  RoPE frequency scaling method, defaults to linear unless specified by the model
         --rope-scale N           RoPE context scaling factor, expands context by a factor of N
         --rope-freq-base N       RoPE base frequency, used by NTK-aware scaling (default: loaded from model)
         --rope-freq-scale N      RoPE frequency scaling factor, expands context by a factor of 1/N
         --yarn-orig-ctx N        YaRN: original context size of model (default: 0 = model training context size)
         --yarn-ext-factor N      YaRN: extrapolation mix factor (default: -1.0, 0.0 = full interpolation)
         --yarn-attn-factor N     YaRN: scale sqrt(t) or attention magnitude (default: 1.0)
         --yarn-beta-slow N       YaRN: high correction dim or alpha (default: 1.0)
         --yarn-beta-fast N       YaRN: low correction dim or beta (default: 32.0)
  -gan,  --grp-attn-n N           group-attention factor (default: 1)
  -gaw,  --grp-attn-w N           group-attention width (default: 512.0)
  -dkvc, --dump-kv-cache          verbose print of the KV cache
  -nkvo, --no-kv-offload          disable KV offload
  -ctk,  --cache-type-k TYPE      KV cache data type for K (default: f16)
  -ctv,  --cache-type-v TYPE      KV cache data type for V (default: f16)

perplexity:

         --all-logits             return logits for all tokens in the batch (default: false)
         --hellaswag              compute HellaSwag score over random tasks from datafile supplied with -f
         --hellaswag-tasks N      number of tasks to use when computing the HellaSwag score (default: 400)
         --winogrande             compute Winogrande score over random tasks from datafile supplied with -f
         --winogrande-tasks N     number of tasks to use when computing the Winogrande score (default: 0)
         --multiple-choice        compute multiple choice score over random tasks from datafile supplied with -f
         --multiple-choice-tasks N
                                  number of tasks to use when computing the multiple choice score (default: 0)
         --kl-divergence          computes KL-divergence to logits provided via --kl-divergence-base
         --ppl-stride N           stride for perplexity calculation (default: 0)
         --ppl-output-type {0,1}  output type for perplexity calculation (default: 0)

parallel:

  -dt,   --defrag-thold N         KV cache defragmentation threshold (default: -1.0, < 0 - disabled)
  -np,   --parallel N             number of parallel sequences to decode (default: 1)
  -ns,   --sequences N            number of sequences to decode (default: 1)
  -cb,   --cont-batching          enable continuous batching (a.k.a dynamic batching) (default: enabled)

multi-modality:

         --mmproj FILE            path to a multimodal projector file for LLaVA. see examples/llava/README.md
         --image FILE             path to an image file. use with multimodal models. Specify multiple times for batching

backend:

         --rpc SERVERS            comma separated list of RPC servers
         --mlock                  force system to keep model in RAM rather than swapping or compressing
         --no-mmap                do not memory-map model (slower load but may reduce pageouts if not using mlock)
         --numa TYPE              attempt optimizations that help on some NUMA systems
                                    - distribute: spread execution evenly over all nodes
                                    - isolate: only spawn threads on CPUs on the node that execution started on
                                    - numactl: use the CPU map provided by numactl
                                  if run without this previously, it is recommended to drop the system page cache before using this
                                  see https://github.com/ggerganov/llama.cpp/issues/1437
  -ngl,  --gpu-layers N           number of layers to store in VRAM
  -ngld, --gpu-layers-draft N     number of layers to store in VRAM for the draft model
  -sm,   --split-mode SPLIT_MODE  how to split the model across multiple GPUs, one of:
                                    - none: use one GPU only
                                    - layer (default): split layers and KV across GPUs
                                    - row: split rows across GPUs
  -ts,   --tensor-split SPLIT     fraction of the model to offload to each GPU, comma-separated list of proportions, e.g. 3,1
  -mg,   --main-gpu i             the GPU to use for the model (with split-mode = none),
                                  or for intermediate results and KV (with split-mode = row) (default: 0)

model:

         --check-tensors          check model tensor data for invalid values (default: false)
         --override-kv KEY=TYPE:VALUE
                                  advanced option to override model metadata by key. may be specified multiple times.
                                  types: int, float, bool, str. example: --override-kv tokenizer.ggml.add_bos_token=bool:false
         --lora FNAME             apply LoRA adapter (implies --no-mmap)
         --lora-scaled FNAME S    apply LoRA adapter with user defined scaling S (implies --no-mmap)
         --lora-base FNAME        optional model to use as a base for the layers modified by the LoRA adapter
         --control-vector FNAME   add a control vector
                                  note: this argument can be repeated to add multiple control vectors
         --control-vector-scaled FNAME SCALE
                                  add a control vector with user defined scaling SCALE
                                  note: this argument can be repeated to add multiple scaled control vectors
         --control-vector-layer-range START END
                                  layer range to apply the control vector(s) to, start and end inclusive
  -m,    --model FNAME            model path (default: models/$filename with filename from --hf-file
                                  or --model-url if set, otherwise models/7B/ggml-model-f16.gguf)
  -md,   --model-draft FNAME      draft model for speculative decoding (default: unused)
  -mu,   --model-url MODEL_URL    model download url (default: unused)
  -hfr,  --hf-repo REPO           Hugging Face model repository (default: unused)
  -hff,  --hf-file FILE           Hugging Face model file (default: unused)
  -hft,  --hf-token TOKEN         Hugging Face access token (default: value from HF_TOKEN environment variable)

retrieval:

         --context-file FNAME     file to load context from (repeat to specify multiple files)
         --chunk-size N           minimum length of embedded text chunks (default: 64)
         --chunk-separator STRING
                                  separator between chunks (default: '
                                  ')

passkey:

         --junk N                 number of times to repeat the junk text (default: 250)
         --pos N                  position of the passkey in the junk text (default: -1)

imatrix:

  -o,    --output FNAME           output file (default: 'imatrix.dat')
         --output-frequency N     output the imatrix every N iterations (default: 10)
         --save-frequency N       save an imatrix copy every N iterations (default: 0)
         --process-output         collect data for the output tensor (default: false)
         --no-ppl                 do not compute perplexity (default: true)
         --chunk N                start processing the input from chunk N (default: 0)

bench:

  -pps                            is the prompt shared across parallel sequences (default: false)
  -npp n0,n1,...                  number of prompt tokens
  -ntg n0,n1,...                  number of text generation tokens
  -npl n0,n1,...                  number of parallel prompts

embedding:

         --embd-normalize         normalisation for embendings (default: 2) (-1=none, 0=max absolute int16, 1=taxicab, 2=euclidean, >2=p-norm)
         --embd-output-format     empty = default, "array" = [[],[]...], "json" = openai style, "json+" = same "json" + cosine similarity matrix
         --embd-separator         separator of embendings (default \n) for example "<#sep#>"

server:

         --host HOST              ip address to listen (default: 127.0.0.1)
         --port PORT              port to listen (default: 8080)
         --path PATH              path to serve static files from (default: )
         --embedding(s)           enable embedding endpoint (default: disabled)
         --api-key KEY            API key to use for authentication (default: none)
         --api-key-file FNAME     path to file containing API keys (default: none)
         --ssl-key-file FNAME     path to file a PEM-encoded SSL private key
         --ssl-cert-file FNAME    path to file a PEM-encoded SSL certificate
         --timeout N              server read/write timeout in seconds (default: 600)
         --threads-http N         number of threads used to process HTTP requests (default: -1)
         --system-prompt-file FNAME
                                  set a file to load a system prompt (initial prompt of all slots), this is useful for chat applications
         --log-format {text,json}
                                  log output format: json or text (default: json)
         --metrics                enable prometheus compatible metrics endpoint (default: disabled)
         --no-slots               disables slots monitoring endpoint (default: enabled)
         --slot-save-path PATH    path to save slot kv cache (default: disabled)
         --chat-template JINJA_TEMPLATE
                                  set custom jinja chat template (default: template taken from model's metadata)
                                  only commonly used templates are accepted:
                                  https://github.com/ggerganov/llama.cpp/wiki/Templates-supported-by-llama_chat_apply_template
  -sps,  --slot-prompt-similarity SIMILARITY
                                  how much the prompt of a request must match the prompt of a slot in order to use that slot (default: 0.50, 0.0 = disabled)


logging:

         --simple-io              use basic IO for better compatibility in subprocesses and limited consoles
  -ld,   --logdir LOGDIR          path under which to save YAML logs (no logging if unset)
         --log-test               Run simple logging test
         --log-disable            Disable trace logs
         --log-enable             Enable trace logs
         --log-file FNAME         Specify a log filename (without extension)
         --log-new                Create a separate new log file on start. Each log file will have unique name: "<name>.<ID>.log"
         --log-append             Don't truncate the old log file.

cvector:

  -o,    --output FNAME           output file (default: 'control_vector.gguf')
         --positive-file FNAME    positive prompts file, one prompt per line (default: 'examples/cvector-generator/positive.txt')
         --negative-file FNAME    negative prompts file, one prompt per line (default: 'examples/cvector-generator/negative.txt')
         --pca-batch N            batch size used for PCA. Larger batch runs faster, but uses more memory (default: 100)
         --pca-iter N             number of iterations used for PCA (default: 1000)
         --method {pca,mean}      dimensionality reduction method to be used (default: pca)
```