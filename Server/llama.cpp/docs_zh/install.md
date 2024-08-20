# 安装预构建的 llama.cpp 版本

## Homebrew

在Mac和Linux上，可以使用Homebrew包管理器通过以下命令安装：

```sh
brew install llama.cpp
```

Homebrew的公式会自动更新以匹配新的`llama.cpp`版本。更多信息请参考：https://github.com/ggerganov/llama.cpp/discussions/7668

## Nix

在Mac和Linux上，可以使用Nix包管理器通过以下命令安装：

```sh
nix profile install nixpkgs#llama-cpp
```

对于支持flake的安装。

或者

```sh
nix-env --file '<nixpkgs>' --install --attr llama-cpp
```

对于不支持flake的安装。

这个表达式会自动更新于[nixpkgs仓库](https://github.com/NixOS/nixpkgs/blob/nixos-24.05/pkgs/by-name/ll/llama-cpp/package.nix#L164)中。

## Flox

在Mac和Linux上，可以使用Flox在Flox环境中安装llama.cpp，通过以下命令：

```sh
flox install llama-cpp
```

Flox遵循nixpkgs构建的llama.cpp。

