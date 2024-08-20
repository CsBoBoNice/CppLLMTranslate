# 调试测试技巧

## 如何在不进行其他操作的情况下运行和执行或调试特定的测试，以缩短反馈循环？

在scripts文件夹中有一个名为debug-test.sh的脚本，它的参数接受一个正则表达式和一个可选的测试号。

例如，运行以下命令将输出一个交互式列表，您可以从中选择一个测试。它具有以下形式：

`debug-test.sh [选项]... <测试正则表达式> <测试号>`

然后它将为您构建和运行在调试器中。

要仅执行测试并获取通过或失败的消息，请运行：

```bash
./scripts/debug-test.sh test-tokenizer
```

要在GDB中测试，使用`-g`标志来启用gdb测试模式。

```bash
./scripts/debug-test.sh -g test-tokenizer

# 一旦进入调试器，即处于箭头提示符下，设置断点的操作可能如下：
>>> b main
```

为了加快测试循环，如果您知道您的测试号，您可以直接运行它，如下所示：

```bash
./scripts/debug-test.sh test 23
```

要获取更多帮助，请使用`debug-test.sh -h`来打印帮助信息。

&nbsp;

### 脚本是如何工作的？

如果您想独立使用脚本中包含的概念，下面简要概述了重要的步骤。

#### 第1步：重置并设置文件夹上下文

从本仓库的根目录开始，创建 `build-ci-debug` 作为我们的构建上下文。

```bash
rm -rf build-ci-debug && mkdir build-ci-debug && cd build-ci-debug
```

#### 第2步：设置构建环境和编译测试二进制文件

以调试模式设置和触发构建。您可以根据需要调整参数，但在这个案例中，这些是合理的默认值。

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DLLAMA_CUDA=1 -DLLAMA_FATAL_WARNINGS=ON ..
make -j
```

#### 第3步：查找所有匹配正则表达式的可用测试

此命令的输出将给出运行GDB所需的命令和参数。

* `-R test-tokenizer` : 查找所有名为 `test-tokenizer*` 的测试文件 (R=正则表达式)
* `-N` : "只显示" 禁用测试执行并显示可以输入到GDB中的测试命令。
* `-V` : 详细模式

```bash
ctest -R "test-tokenizer" -V -N
```

这可能会返回类似下面的输出（仅关注需要关注的要点）：

```bash
...
1: 测试命令: ~/llama.cpp/build-ci-debug/bin/test-tokenizer-0 "~/llama.cpp/tests/../models/ggml-vocab-llama-spm.gguf"
1: 工作目录: .
标签: main
  测试 #1: test-tokenizer-0-llama-spm
...
4: 测试命令: ~/llama.cpp/build-ci-debug/bin/test-tokenizer-0 "~/llama.cpp/tests/../models/ggml-vocab-falcon.gguf"
4: 工作目录: .
标签: main
  测试 #4: test-tokenizer-0-falcon
...
```

#### 步骤 4: 识别调试用的测试命令

对于上面的测试 #1，我们可以得知以下两段相关信息：
* 测试二进制文件：`~/llama.cpp/build-ci-debug/bin/test-tokenizer-0`
* 测试 GGUF 模型：`~/llama.cpp/tests/../models/ggml-vocab-llama-spm.gguf`

#### 步骤 5: 在测试命令上运行 GDB

根据上面 ctest '测试命令' 的报告，我们可以使用以下命令来运行一个 gdb 会话：

```bash
gdb --args ${测试二进制文件} ${测试 GGUF 模型}
```

示例：

```bash
gdb --args ~/llama.cpp/build-ci-debug/bin/test-tokenizer-0 "~/llama.cpp/tests/../models/ggml-vocab-llama-spm.gguf"
```

