#ifndef _TRANSLATION_MANAGER_H_
#define _TRANSLATION_MANAGER_H_

#include "cJSON.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

// 定义文件类型的枚举
enum class FileType
{
    TXT,
    MD,
    RST,
    HPP,
    UNKNOWN
};

// 定义结构体来保存文件路径和分割后的文本内容
struct FileContent
{
    std::string path;
    std::vector<std::string> content;
};

class TranslationManager
{
private:
public:
    int paragraph_effective = 1 * 512;  // 段落有效值
    int paragraph_min = 1 * 1024 + 512; // 段落最小值(根据n_batch计算)
    int paragraph_max = 2 * 1024 + 512; // 段落最大值(根据n_batch计算)

    const char *chatPrefix = "Original text to be translated:\r\n"; // 翻译前缀

    // 输出文件路径 翻译途中双语对比
    std::string output_comparison_file_path = "../file_processing/datasheet_output/output_comparison.md";

    std::filesystem::path directory = "../file_processing/datasheet_input/";              // 原输入路径
    std::filesystem::path directory_output = "../file_processing/datasheet_output/";      // 输出路径
    std::filesystem::path directory_cut = "../file_processing/datasheet_output/docs_cut"; // 切割输出路径
    std::filesystem::path directory_en = "../file_processing/datasheet_output/docs_en";   // 翻译输出路径(中英对照)

    std::string progress_file = "../file_processing/datasheet_output/progress.json"; // 进度文件
    std::string paragraph_info_file = "../prompts/paragraph_info.json"; // 段落分割阈值文件

    std::string nice_input_rst_1 = "../prompts/nice_input_rst_1.txt";   // 优质提问
    std::string nice_output_rst_1 = "../prompts/nice_output_rst_1.txt"; // 优质回答
    std::string nice_input_rst_2 = "../prompts/nice_input_rst_2.txt";   // 优质提问
    std::string nice_output_rst_2 = "../prompts/nice_output_rst_2.txt"; // 优质回答
    std::string nice_input_rst_3 = "../prompts/nice_input_rst_3.txt";   // 优质提问
    std::string nice_output_rst_3 = "../prompts/nice_output_rst_3.txt"; // 优质回答
    std::string chat_prefix_rst = "../prompts/chat_prefix_rst.txt";     // 提问前缀
    std::string chat_suffix_rst = "../prompts/chat_suffix_rst.txt";     // 提问后缀
    std::string systemInfo_rst = "../prompts/systemInfo_rst.txt";       // 系统提示

    std::string string_nice_input_rst_1;  // 优质提问
    std::string string_nice_output_rst_1; // 优质回答
    std::string string_nice_input_rst_2;  // 优质提问
    std::string string_nice_output_rst_2; // 优质回答
    std::string string_nice_input_rst_3;  // 优质提问
    std::string string_nice_output_rst_3; // 优质回答
    std::string string_chat_prefix_rst;   // 提问前缀
    std::string string_chat_suffix_rst;   // 提问后缀
    std::string string_systemInfo_rst;    // 系统提示

    std::string nice_input_md = "../prompts/nice_input_md.txt";         // 优质提问
    std::string nice_output_md = "../prompts/nice_output_md.txt";       // 优质回答
    std::string chat_prefix_md = "../prompts/chat_prefix_md.txt"; // 提问前缀
    std::string chat_suffix_md = "../prompts/chat_suffix_md.txt";       // 提问后缀
    std::string systemInfo_md = "../prompts/systemInfo_md.txt";         // 系统提示

    std::string string_nice_input_md;  // 优质提问
    std::string string_nice_output_md; // 优质回答
    std::string string_chat_prefix_md; // 提问前缀
    std::string string_chat_suffix_md; // 提问后缀
    std::string string_systemInfo_md;  // 系统提示

    std::string nice_input_hpp = "../prompts/nice_input_hpp.txt";         // 优质提问
    std::string nice_output_hpp = "../prompts/nice_output_hpp.txt";       // 优质回答
    std::string chat_prefix_hpp = "../prompts/chat_prefix_hpp.txt"; // 提问前缀
    std::string chat_suffix_hpp = "../prompts/chat_suffix_hpp.txt";       // 提问后缀
    std::string systemInfo_hpp = "../prompts/systemInfo_hpp.txt";         // 系统提示

    std::string string_nice_input_hpp;  // 优质提问
    std::string string_nice_output_hpp; // 优质回答
    std::string string_chat_prefix_hpp; // 提问前缀
    std::string string_chat_suffix_hpp; // 提问后缀
    std::string string_systemInfo_hpp;  // 系统提示

    std::string Separator_cut = "*-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-**-*-*";

    std::vector<FileContent> translation_cache; // 所有文件待翻译的内容

    std::ostringstream my_output_ss;

    size_t m_file_index = 0;      // 文件计数
    size_t m_paragraph_index = 0; // 段落计数

    unsigned int error_count = 0;
    unsigned int restart_count = 0; // 重启计数 避免多次运行后有脏回答污染后续翻译

    TranslationManager();  // 构造函数
    ~TranslationManager(); // 析构函数

    void removeTrailingImEndTag(std::string &str,std::string str_remove);
    bool isTitle(const std::string &line);
    void ProcessFilesCut(const std::filesystem::path &directory, const std::filesystem::path &target_directory,
                         const std::filesystem::path &root_directory);
    FileContent getFileContent(const std::filesystem::directory_entry &entry);
    void ProcessFilesRecursive(const std::filesystem::path &directory, const std::filesystem::path &target_directory,
                               const std::filesystem::path &root_directory, std::vector<FileContent> &result);
    void ReadTranslated(const std::filesystem::path &directory, const std::filesystem::path &root_directory,
                        std::vector<FileContent> &result);
    void SaveToFilesystem(const std::vector<FileContent> &result, const std::filesystem::path &out_root_directory, size_t index);
    bool fileExists(const std::string &filename);
    void writeJsonToFile(const std::string &filename, size_t file_index, size_t paragraph_index);
    bool readJsonFromFile(const std::string &filename, size_t &file_index, size_t &paragraph_index);
    std::string readFile(const std::string &filename);
    void init_systemInfo(void);
    FileType checkFileType(const std::string &filename);

    bool readJsonFromFile_paragraph(const std::string &filename);
};

#endif // TRANSLATIONMANAGER_H
