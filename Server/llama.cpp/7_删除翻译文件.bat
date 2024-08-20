chcp 65001
@echo off
echo 正在删除文件夹...

:: 删除datasheet_output文件夹及其所有内容
rd /s /q ..\file_processing\datasheet_output

echo 删除完成。

