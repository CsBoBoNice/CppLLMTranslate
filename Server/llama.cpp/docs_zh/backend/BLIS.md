BLIS 安装手册
------------------------

BLIS 是一个可移植的高性能 BLAS 类似密集线性代数库的软件框架。它获得了奖项和认可，包括 2023 年詹姆斯·H·威尔金森数值软件奖和 2020 年 SIAM 高性能计算活动组最佳论文奖。BLIS 提供了一个新的 BLAS 类似 API 和一个传统 BLAS 程序调用兼容层。它还提供了基于对象 API、类型化 API、BLAS 和 CBLAS 兼容层等特性。

项目网址：https://github.com/flame/blis

### 准备：

编译 BLIS：

```bash
git clone https://github.com/flame/blis
cd blis
./configure --enable-cblas -t openmp,pthreads auto
# 默认安装到 /usr/local/
make -j
```

安装 BLIS：

```bash
sudo make install
```

我们推荐使用 openmp，因为它更容易修改使用的核心。

### llama.cpp 编译

Makefile：

```bash
make GGML_BLIS=1 -j
# make GGML_BLIS=1 llama-benchmark-matmult
```

CMake：

```bash
mkdir build
cd build
cmake -DGGML_BLAS=ON -DGGML_BLAS_VENDOR=FLAME ..
make -j
```

### llama.cpp执行

根据BLIS文档，我们可以设置以下环境变量来修改openmp的行为：

```bash
export GOMP_CPU_AFFINITY="0-19"
export BLIS_NUM_THREADS=14
```

然后以正常方式运行二进制文件。


### Intel特定问题

有些人可能会收到错误消息，说找不到`libimf.so`。请参考这个[Stack Overflow页面](https://stackoverflow.com/questions/70687930/intel-oneapi-2022-libimf-so-no-such-file-or-directory-during-openmpi-compila)。


### 参考文献：

1. https://github.com/flame/blis#getting-started
2. https://github.com/flame/blis/blob/master/docs/Multithreading.md

