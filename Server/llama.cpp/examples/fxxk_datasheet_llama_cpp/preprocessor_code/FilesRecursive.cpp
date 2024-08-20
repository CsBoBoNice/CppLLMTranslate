#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>



// 检查行是否是标题的函数
bool isTitle(const std::string& line) {
    size_t firstHash = line.find('#');
    return firstHash == 0 || (firstHash != std::string::npos && firstHash == line.find_first_not_of(' ') && line[firstHash + 1] == ' ');
}

// 递归遍历文件夹中的文件,按.md区分并按标题分割，非.md则按目录结构拷贝
// directory为输入文件夹 target_directory为输出文件夹 directory与root_directory相同 
void ProcessFiles(const std::filesystem::path& directory, const std::filesystem::path& target_directory, const std::filesystem::path& root_directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);
        std::filesystem::path target_path = target_directory / relative_path;

        if (std::filesystem::is_directory(entry.status())) {
            // 创建对应的子目录
            if (!std::filesystem::exists(target_path)) {
                std::filesystem::create_directory(target_path);
            }
            // 递归处理子目录
            ProcessFiles(entry.path(), target_directory, root_directory);
        }
        else {
            // 确保目标文件的目录存在
            if (!std::filesystem::exists(target_path.parent_path())) {
                std::filesystem::create_directories(target_path.parent_path());
            }

            if (entry.path().extension() == ".md") {

                std::ifstream inputFile(entry.path());
                std::ofstream outputFile(target_path);
                if (!inputFile.is_open() || !outputFile.is_open()) {
                    std::cerr << "open file error" << std::endl;
                    inputFile.close();
                    outputFile.close();
                    continue;
                }

                // 将markdown格式文件按标题分割
                std::string line;
                bool inCodeBlock = false;
                while (std::getline(inputFile, line)) {
                    if (line.find("```") == 0) {
                        inCodeBlock = !inCodeBlock;
                    }

                    if (!inCodeBlock && isTitle(line)) {
                        outputFile << "--------------------------------------------------------\n";
                    }

                    outputFile << line << "\n";
                }

                inputFile.close();
                outputFile.close();
            }
            else {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path, std::filesystem::copy_options::overwrite_existing);
            }
        }
    }
}


// 定义结构体来保存文件路径和分割后的文本内容
struct FileContent {
    std::string path;
    std::vector<std::string> content;
};

// 递归遍历文件夹中的文件,按.md区分文件 按分隔符与标题段长度划分段落 保存至std::vector<FileContent>& result
void ProcessFilesRecursive(const std::filesystem::path& directory, const std::filesystem::path& target_directory, const std::filesystem::path& root_directory, std::vector<FileContent>& result) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);
        std::filesystem::path target_path = target_directory / relative_path;

        if (std::filesystem::is_directory(entry.status())) {
            // 创建对应的子目录
            if (!std::filesystem::exists(target_path)) {
                std::filesystem::create_directory(target_path);
            }

            // 递归处理子目录
            ProcessFilesRecursive(entry.path(), target_directory, root_directory, result);
        }
        else {
            // 确保目标文件的目录存在
            if (!std::filesystem::exists(target_path.parent_path())) {
                std::filesystem::create_directories(target_path.parent_path());
            }

            if (entry.path().extension() == ".md") {
                std::ifstream input(entry.path());
                if (!input.is_open()) {
                    std::cerr << "无法打开文件: " << entry.path() << std::endl;
                    continue;
                }

                FileContent fileContent;
                fileContent.path = relative_path.string();

                std::string paragraph;
                std::string line;

                while (std::getline(input, line))
                {
                    // 检查是否是特定分隔符
                    if (line == "--------------------------------------------------------")
                    {
                        // 当遇到分隔行时，如果段落包含字母，则保存当前段落
                        if (paragraph.size() >= 512 && !paragraph.empty() && std::any_of(paragraph.begin(), paragraph.end(), [](unsigned char c) { return std::isalpha(c); })) {
                            fileContent.content.push_back(paragraph + "\n"); // 添加换行符
                            paragraph.clear();
                        }
                    }
                    else if (paragraph.size() >= 2 && ((paragraph[paragraph.size() - 1] == '\n' && paragraph[paragraph.size() - 2] == '.' && paragraph.size() >= 2048) || (paragraph[paragraph.size() - 1] == '\n' && paragraph.size() >= 4096)))
                    {
                        // 检查是否是英文句号后紧跟换行符 读取的字节数大于 2048 或者 大于 2560 以 回车结尾
                        paragraph += line;
                        fileContent.content.push_back(paragraph + "\n"); // 添加换行符
                        paragraph.clear();
                    }
                    else
                    {
                        // 否则，添加到当前段落
                        line += "\n";
                        paragraph += line;
                    }
                }

                // 如果文件末尾没有分隔行，保存最后的段落
                if (!paragraph.empty())
                {
                    fileContent.content.push_back(paragraph + "\nLife is actually like the weather, with its sunny days, cloudy days, and occasional rain showers. It's the natural order of things. Life isn't simple, but we should strive to simplify it as much as possible.\n"); // 添加换行符
                }

                input.close();
                result.push_back(fileContent);
            }
            else {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path, std::filesystem::copy_options::overwrite_existing);
            }

        }

    }
}
/*
void ProcessFilesRecursive(const std::filesystem::path& directory, const std::filesystem::path& root_directory, std::vector<FileContent>& result) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);

        if (std::filesystem::is_directory(entry.status())) {
            // 递归处理子目录
            ProcessFilesRecursive(entry.path(), root_directory, result);
        }
        else if (entry.path().extension() == ".md") {
            std::ifstream input(entry.path());
            if (!input.is_open()) {
                std::cerr << "无法打开文件: " << entry.path() << std::endl;
                continue;
            }

            FileContent fileContent;
            fileContent.path = relative_path.string();
            std::string line;
            std::string chunk;
            int count = 0;

            while (std::getline(input, line)) {
                chunk += line + "\n";
                ++count;
                if (count == 10) {
                    fileContent.content.push_back(chunk);
                    chunk.clear();
                    count = 0;
                }
            }

            // 如果最后一部分不足10行，也加入结果
            if (!chunk.empty()) {
                fileContent.content.push_back(chunk);
            }

            result.push_back(fileContent);
            input.close();
        }
    }
}
*/

int main() {
    std::vector<FileContent> result;
    std::filesystem::path directory = "D:/code/FilesRecursive/FilesRecursive/docs"; // 输入路径
    std::filesystem::path directory_cut = "D:/code/FilesRecursive/FilesRecursive/docs_cut"; // 输入路径
    std::filesystem::path directory_en = "D:/code/FilesRecursive/FilesRecursive/docs_en"; // 输出路径


    ProcessFiles(directory, directory_cut, directory);
    ProcessFilesRecursive(directory_cut, directory_en, directory_cut, result);

    // 使用数组下标的方式遍历result并将内容保存至out_root_directory文件夹中
    for (size_t directory_index = 0; directory_index < result.size(); ++directory_index) {
        std::filesystem::path out_path = directory_en / result[directory_index].path;
        // 创建输出文件的目录
        if (!std::filesystem::exists(out_path.parent_path())) {
            std::filesystem::create_directories(out_path.parent_path());
        }
        std::ofstream output(out_path);
        if (!output.is_open()) {
            std::cerr << "无法创建文件: " << out_path << std::endl;
            continue;
        }
        // 使用传统的for循环遍历content向量
        for (size_t file_index = 0; file_index < result[directory_index].content.size(); ++file_index) {
            output << result[directory_index].content[file_index];
        }
        output.close();
    }

    return 0;
}