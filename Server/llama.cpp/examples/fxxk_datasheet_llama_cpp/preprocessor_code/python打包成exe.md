python打包成exe




创建环境
```
conda create --name py_to_txt python=3.11
```

要将Python代码打包成exe文件，可以使用PyInstaller工具。以下是使用PyInstaller打包Python代码为exe的基本步骤：

1. 安装PyInstaller：

```bash
pip install pyinstaller
```

安装完成后，检查是否安装成功：

```
pyinstaller --version
```



指令解释：

```
pyinstaller -F [-w] *.py -n [自定义名称]
```

```
pyinstaller
[-F/-D]       # [产生当个可执行文件/产生一个目录（包含多个文件）作为可执行文件]
[-w/-c]       # [去掉命令行弹窗/显示命令行弹窗]
-i icon.ico   # 指定exe显示图标
demo.py     #打包的python文件


-F: 打包成一个exe文件

-w: 取消控制台显示，即运行时不弹出黑窗口；建议不使用-w,方便查看打印的信息或者出错信息

-n: 自定义.exe文件的名称；如果不使用-n,则默认是脚本脚本文件的名称

```



使用PyInstaller打包Python脚本：

```
pyinstaller -F pdf_to_txt_2.py -n pdf_to_txt.exe
```



执行完毕后，在`dist`文件夹中找到`example.exe`并运行它。 



