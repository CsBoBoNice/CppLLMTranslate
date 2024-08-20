# Android

## 在Termux上构建Android

[Termux](https://github.com/termux/termux-app#installation) 是在Android设备上执行 `llama.cpp` 的方法（无需root权限）。

```bash
apt update && apt upgrade -y
apt install git make cmake
```

建议将您的模型移动到 `~/` 目录以获得最佳性能：

```bash
cd storage/downloads
mv model.gguf ~/
```

[获取代码](https://github.com/ggerganov/llama.cpp#get-the-code) & [遵循Linux构建说明](https://github.com/ggerganov/llama.cpp#build) 来构建 `llama.cpp`。

## 使用Android NDK构建项目

获取 [Android NDK](https://developer.android.com/ndk) 并使用CMake进行构建。

在您的计算机上执行以下命令以避免将NDK下载到您的手机。您也可以在Termux中执行这些操作：
```bash
$ mkdir build-android
$ cd build-android
$ export NDK=<your_ndk_directory>
$ cmake -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-23 -DCMAKE_C_FLAGS=-march=armv8.4a+dotprod ..
$ make
```

在您的设备上安装 [termux](https://github.com/termux/termux-app#installation) 并运行 `termux-setup-storage` 以访问您的SD卡（如果Android 11+，则运行命令两次）。

最后，将这些构建的 `llama` 二进制文件和模型文件复制到您的设备存储中。由于Android SD卡中的文件权限无法更改，您可以将可执行文件复制到 `/data/data/com.termux/files/home/bin` 路径，然后在Termux中执行以下命令以添加可执行权限：

(假设您已使用 `adb push` 将构建的可执行文件推送到 /sdcard/llama.cpp/bin 路径)
```bash
$cp -r /sdcard/llama.cpp/bin /data/data/com.termux/files/home/
$cd /data/data/com.termux/files/home/bin
$chmod +x ./*
```

下载模型 [llama-2-7b-chat.Q4_K_M.gguf](https://huggingface.co/TheBloke/Llama-2-7B-Chat-GGUF/blob/main/llama-2-7b-chat.Q4_K_M.gguf)，并将其推送到 `/sdcard/llama.cpp/`，然后将其移动到 `/data/data/com.termux/files/home/model/`

```bash
$mv /sdcard/llama.cpp/llama-2-7b-chat.Q4_K_M.gguf /data/data/com.termux/files/home/model/
```

现在，你可以开始聊天了：

```bash
$cd /data/data/com.termux/files/home/bin
$./llama-cli -m ../model/llama-2-7b-chat.Q4_K_M.gguf -n 128 -cml
```

这是一个在Pixel 5手机上运行的交互会话的演示：

[视频链接](https://user-images.githubusercontent.com/271616/225014776-1d567049-ad71-4ef2-b050-55b0b3b9274c.mp4)

