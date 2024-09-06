/*
 * @Date: 2024-08-31 11:11:00
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-31 11:51:55
 * @FilePath: /CppLLMTranslate/GUI/qt/FileManager.h
 */
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "cJSON.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

// 定义文件类型的枚举
enum class FileType { TXT, MD, RST, HPP, UNKNOWN };

// 定义结构体来保存文件路径和分割后的文本内容
struct FileContent {
    std::string path;
    std::vector<std::string> content;
};

class FileManager {
  public:
    int paragraph_effective = 512;  // 段落有效值
    int paragraph_min = 4096; // 段落最小值
    int paragraph_max = 6144; // 段落最大值

    std::filesystem::path directory = "/home/csbobo/code/CppLLMTranslate/GUI/qt/build/ttt/";              // 原输入路径
    std::filesystem::path directory_output = "/home/csbobo/code/CppLLMTranslate/GUI/qt/build/";      // 输出路径
    std::filesystem::path directory_cut = "/home/csbobo/code/CppLLMTranslate/GUI/qt/build/docs_cut/"; // 切割输出路径
    std::filesystem::path directory_en = "/home/csbobo/code/CppLLMTranslate/GUI/qt/build/docs_en/"; // 翻译输出路径(中英对照)
    std::filesystem::path directory_ok = "/home/csbobo/code/CppLLMTranslate/GUI/qt/build/docs_ok/"; // 翻译输出路径(中文)

    std::string progress_file = "../file_processing/datasheet_output/progress.json"; // 进度文件
    std::string paragraph_info_file = "../prompts/paragraph_info.json";              // 段落分割阈值文件

    std::string Separator_cut = "*-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-*";

    std::vector<FileContent> translation_cache; // 所有文件待翻译的内容

    size_t m_file_index = 0;      // 文件计数
    size_t m_paragraph_index = 0; // 段落计数

    bool m_cut_sign=false;

  public:
    FileManager();

    // 移除末尾的结束标签。
    void removeTrailingImEndTag(std::string &str, std::string str_remove);

    // 判断是否是markdown标题
    bool isTitle(const std::string &line);

    // 处理文件切割
    // 递归遍历文件夹中的文件,按文件后最区分并按标题分割，非特定文件则按目录结构拷贝
    // directory 为输入文件夹 target_directory 为输出文件夹 directory 与 root_directory 相同
    void ProcessFilesCut(const std::filesystem::path &directory,
                         const std::filesystem::path &target_directory,
                         const std::filesystem::path &root_directory);

    // 函数用于读取文件内容并返回FileContent结构体
    FileContent getFileContent(const std::filesystem::directory_entry &entry);

    // 函数用于读取文件内容并返回FileContent结构体
    FileContent getFileContent_ok(const std::filesystem::directory_entry &entry);

    // 将切割好的段落放入缓冲区 非翻译文件直接拷贝
    void ProcessFilesRecursive(const std::filesystem::path &directory,
                               const std::filesystem::path &target_directory,
                               const std::filesystem::path &root_directory,
                               std::vector<FileContent> &result);

    // 读取已翻译内容 保存至std::vector<FileContent>& result
    void ReadTranslated(const std::filesystem::path &directory,
                        const std::filesystem::path &target_directory,
                        const std::filesystem::path &root_directory,
                        std::vector<FileContent> &result);

    // 将result中的内容保存至指定文件夹的函数
    void SaveToFilesystem(const std::vector<FileContent> &result,
                          const std::filesystem::path &out_root_directory,
                          size_t index);

    // 将翻译好的中英比对带切割符号的文件 提取中文 保存到指定文件夹
    void SaveTranslatedFiles(const std::vector<FileContent> &result, const std::filesystem::path &out_root_directory);

    void CleanAll();

    // 判断字符串末尾的文件类型
    FileType checkFileType(const std::string &filename);
};

#endif // FILEMANAGER_H
