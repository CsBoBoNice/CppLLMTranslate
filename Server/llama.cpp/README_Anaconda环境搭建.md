# Anaconda环境搭建

## Anaconda介绍

Anaconda是一个Python编程语言的开发环境，它包含了众多科学计算和数据分析库
Anaconda提供了一个方便的环境管理器

安装好Anaconda后会自带一个Anaconda Prompt用于执行命令行

使用conda命令很方便管理代码运行环境

## Anaconda 镜像地址

清华大学开源软件镜像站:
```
https://mirrors.tuna.tsinghua.edu.cn/help/anaconda/

```

Anaconda清华大学开源软件镜像站下载地址:
```
https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/
```

*选择下载Anaconda3-2023.09-0-Windows-x86_64.exe*

>注意安装时选上加入环境变量

若未选择则手动加入添加环境变量
添加环境变量:

```
C:\ProgramData\anaconda3
C:\ProgramData\anaconda3\Scripts\
C:\ProgramData\anaconda3\condabin
C:\ProgramData\anaconda3\Library\bin
C:\ProgramData\anaconda3\Library\mingw-w64\bin
```

在命令行窗口输入conda测试



### anaconda换源

参考网址
```
https://blog.csdn.net/weixin_49703503/article/details/128360909
```

打开Anaconda Prompt终端输入
在家目录生成名为 .condarc 的配置文件：
```
conda config --set show_channel_urls yes，
```

修改文件
```
C:\Users\csbobo\.condarc
```
改为

```
channels:
  - defaults
show_channel_urls: true
default_channels:
  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/r
  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/msys2
custom_channels:
  conda-forge: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  msys2: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  bioconda: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  menpo: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  pytorch: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  pytorch-lts: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
  simpleitk: https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud
```

打开终端运行

```
python -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

## conda命令

### 创建名为 env_name 的虚拟环境

创建名为 env_name 的虚拟环境：
```
conda create --name env_name
```

创建名为 env_name 的虚拟环境并同时安装 python3.11 ：
```
conda create --name env_name python=3.11
```

删除名为 env_name 的虚拟环境：
```
conda remove --name env_name --all
```

复制名为 env_name 的虚拟环境：
```
conda create --name env_name_old --clone env_name_new
```
PS：Anaconda没有重命名虚拟环境的操作，若要重命名虚拟环境，需要结合复制和删除虚拟环境两个命令实现。

### 激活虚拟环境

激活名为 env_name 的虚拟环境：
```
conda activate env_name
```

### 查看当前虚拟环境列表

```
conda env list 
```

或 
```
conda info -e
```

### 给虚拟环境装包
指定虚拟环境名进行装包：
```
conda install -n env_name package_name
```

激活虚拟环境，并在该虚拟环境下装包：
```
conda activate env_name

conda install package_name
```

安装指定版本号的包：
```
conda install peckage_name==x.x
```