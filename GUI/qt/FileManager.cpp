#include "FileManager.h"

FileManager::FileManager() {}

void FileManager::removeTrailingImEndTag(std::string &str, std::string str_remove)
{
    // 查找"<|im_end|>"标签的位置
    size_t pos = str.rfind(str_remove);
    if (pos != std::string::npos) {
        // 如果找到了，就删除它
        str.erase(pos, str_remove.length());
    }
}

bool FileManager::isTitle(const std::string &line)
{
    size_t firstHash = line.find('#');
    return firstHash == 0 ||
        (firstHash != std::string::npos && firstHash == line.find_first_not_of(' ') && line[firstHash + 1] == ' ');
}

// 处理文件切割
// 递归遍历文件夹中的文件,按文件后最区分并按标题分割，非特定文件则按目录结构拷贝
// directory 为输入文件夹 target_directory 为输出文件夹 directory 与 root_directory 相同
void FileManager::ProcessFilesCut(const std::filesystem::path &directory,
                                  const std::filesystem::path &target_directory,
                                  const std::filesystem::path &root_directory)
{
    // 创建输出目录
    if (!std::filesystem::exists(directory_output)) {
        std::filesystem::create_directory(directory_output);
    }

    // 创建输出目录
    if (!std::filesystem::exists(target_directory)) {
        std::filesystem::create_directory(target_directory);
    }

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);
        std::filesystem::path target_path = target_directory / relative_path;

        if (std::filesystem::is_directory(entry.status())) {
            // 创建对应的子目录
            if (!std::filesystem::exists(target_path)) {
                std::filesystem::create_directory(target_path);
            }
            // 递归处理子目录
            ProcessFilesCut(entry.path(), target_directory, root_directory);
        } else {
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
                        outputFile << Separator_cut << "\n";
                    }

                    outputFile << line << "\n";
                }

                inputFile.close();
                outputFile.close();
            } else if (entry.path().extension() == ".rst") {

                std::ifstream inputFile(entry.path());
                std::ofstream outputFile(target_path);

                if (!inputFile.is_open() || !outputFile.is_open()) {
                    std::cerr << "open file error" << std::endl;
                    inputFile.close();
                    outputFile.close();
                    continue;
                }

                std::string line = "";
                std::string line_last = "";
                std::string chapter;
                std::regex title_pattern("^[=\\-`:\\'\\\"~\\^\\_\\*\\+\\#\\<\\>]+$");

                while (std::getline(inputFile, line)) {
                    // 检测章节标题
                    if (std::regex_search(line, title_pattern) && line.size() >= line_last.size()) {
                        if (!chapter.empty()) {

                            outputFile << Separator_cut << "\n";
                            outputFile << chapter;
                            chapter.clear();
                        }
                    }

                    chapter += line_last + "\n";
                    line_last = line;
                }

                chapter += line_last + "\n";

                // 添加最后一个章节的内容（如果有的话）
                if (!chapter.empty()) {
                    outputFile << Separator_cut << "\n";
                    outputFile << chapter;
                }

                inputFile.close();
                outputFile.close();

            } else if (entry.path().extension() == ".h") {

                std::ifstream inputFile(entry.path());
                std::ofstream outputFile(target_path);

                if (!inputFile.is_open() || !outputFile.is_open()) {
                    std::cerr << "open file error" << std::endl;
                    inputFile.close();
                    outputFile.close();
                    continue;
                }

                std::string line = "";

                while (std::getline(inputFile, line)) {
                    // 检查空行 或 全为空格的行
                    if (line.empty()) {
                        outputFile << "\n" << Separator_cut << "\n";
                    } else {
                        outputFile << line << "\n";
                    }
                }

                inputFile.close();
                outputFile.close();

            } else {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path,
                                           std::filesystem::copy_options::overwrite_existing);
            }
        }
    }
}

// 函数用于读取文件内容并返回FileContent结构体
FileContent FileManager::getFileContent(const std::filesystem::directory_entry &entry)
{
    FileContent fileContent;

    std::ifstream input(entry.path());
    if (!input.is_open()) {
        // std::cerr << "无法打开文件: " << entry.path() << std::endl;
        return fileContent;
    }

    std::string paragraph;
    std::string line;

    while (std::getline(input, line)) {
        // 检查是否是特定分隔符
        if (line == Separator_cut) {
            // // 当遇到分隔行时，如果段落包含字母，则保存当前段落
            // if (paragraph.size() >= paragraph_effective && !paragraph.empty() &&
            //     std::any_of(paragraph.begin(), paragraph.end(), [](unsigned char c) { return std::isalpha(c); })) {
            //     fileContent.content.push_back(paragraph); // 添加换行符
            //     paragraph.clear();
            // }

            // 当遇到分隔行时，判断长度保存段落
            if (paragraph.size() >= paragraph_effective) {
                fileContent.content.push_back(paragraph); // 添加换行符
                paragraph.clear();
            }
        } else if (paragraph.size() >= paragraph_effective &&
                   ((line.empty() && paragraph.size() >= paragraph_min) ||
                    (paragraph[paragraph.size() - 1] == '\n' && paragraph[paragraph.size() - 2] == '.' &&
                     paragraph.size() >= paragraph_min) ||
                    (paragraph[paragraph.size() - 1] == '\n' && paragraph.size() >= paragraph_max))) {
            // 检查是否是英文句号后紧跟换行符 读取的字节数大于 paragraph_min 或者 大于 paragraph_max 以 回车结尾
            fileContent.content.push_back(paragraph); // 添加换行符
            paragraph.clear();

            line += "\n";
            paragraph += line;
        } else {
            // 否则，添加到当前段落
            line += "\n";
            paragraph += line;
        }
    }

    // 如果文件末尾没有分隔行，保存最后的段落
    if (!paragraph.empty()) {
        if (paragraph.size() > 64) {
            fileContent.content.push_back(paragraph); // 添加换行符
        } else {
            fileContent.content.push_back(
                paragraph +
                "\nLife is actually like the weather, with its sunny days, cloudy days, and occasional "
                "rain showers. It's the natural order of things. Life isn't simple, but we should strive "
                "to simplify it as much as possible.\n"); // 添加换行符
        }
    }

    input.close();
    return fileContent;
}

// 函数用于读取文件内容并返回FileContent结构体
FileContent FileManager::getFileContent_ok(const std::filesystem::directory_entry &entry)
{
    FileContent fileContent;

    std::ifstream input(entry.path());
    if (!input.is_open())
    {
        // std::cerr << "无法打开文件: " << entry.path() << std::endl;
        return fileContent;
    }

    std::string paragraph;
    std::string line;

    while (std::getline(input, line))
    {
        // 检查是否是特定分隔符
        if (line == Separator_cut)
        {
            if (paragraph.size() >= 2) // 判断是否有效
            {
                // 当遇到分隔行时，增加一个段落
                fileContent.content.push_back(paragraph); // 添加换行符
                paragraph.clear();
            }
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
        fileContent.content.push_back(paragraph); // 添加换行符
    }

    input.close();
    return fileContent;
}

// 将切割好的段落放入缓冲区 非翻译文件直接拷贝
void FileManager::ProcessFilesRecursive(const std::filesystem::path &directory,
                                        const std::filesystem::path &target_directory,
                                        const std::filesystem::path &root_directory,
                                        std::vector<FileContent> &result)
{
    // 创建输出目录
    if (!std::filesystem::exists(target_directory)) {
        std::filesystem::create_directory(target_directory);
    }

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);
        std::filesystem::path target_path = target_directory / relative_path;

        if (std::filesystem::is_directory(entry.status())) {
            // 创建对应的子目录
            if (!std::filesystem::exists(target_path)) {
                std::filesystem::create_directory(target_path);
            }

            // 递归处理子目录
            ProcessFilesRecursive(entry.path(), target_directory, root_directory, result);
        } else {
            // 确保目标文件的目录存在
            if (!std::filesystem::exists(target_path.parent_path())) {
                std::filesystem::create_directories(target_path.parent_path());
            }

            if (entry.path().filename() != "CMakeLists.txt" &&
                (entry.path().extension() == ".md" || entry.path().extension() == ".txt" ||
                 entry.path().extension() == ".rst" || entry.path().extension() == ".h")) {
                FileContent fileContent;
                fileContent = getFileContent_ok(entry);
                fileContent.path = relative_path.string();

                if (fileContent.content.size() == 0) {
                    continue;
                }

                result.push_back(fileContent);
            } else {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path,
                                           std::filesystem::copy_options::overwrite_existing);
            }
        }
    }
}

// 读取已翻译内容 保存至std::vector<FileContent>& result
void FileManager::ReadTranslated(const std::filesystem::path &directory,
                                 const std::filesystem::path &root_directory,
                                 std::vector<FileContent> &result)
{
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);

        if (std::filesystem::is_directory(entry.status())) {
            // 递归处理子目录
            ReadTranslated(entry.path(), root_directory, result);
        } else {
            if (entry.path().filename() != "CMakeLists.txt" &&
                (entry.path().extension() == ".md" || entry.path().extension() == ".txt" ||
                 entry.path().extension() == ".rst" || entry.path().extension() == ".h")) {
                FileContent fileContent;
                fileContent = getFileContent(entry);
                fileContent.path = relative_path.string();

                if (fileContent.content.size() == 0) {
                    continue;
                }

                result.push_back(fileContent);
            }
        }
    }
}

// 将result中的内容保存至指定文件夹的函数
void FileManager::SaveToFilesystem(const std::vector<FileContent> &result,
                                   const std::filesystem::path &out_root_directory,
                                   size_t index)
{
    // 使用数组下标的方式遍历result并将内容保存至out_root_directory文件夹中
    for (size_t file_index = 0; file_index < result.size(); ++file_index) {
        std::filesystem::path out_path = out_root_directory / result[file_index].path;
        // 创建输出文件的目录
        if (!std::filesystem::exists(out_path.parent_path())) {
            std::filesystem::create_directories(out_path.parent_path());
        }

        if (index == file_index) {
            std::ofstream output(out_path);
            if (!output.is_open()) {
                std::cerr << "无法创建文件: " << out_path << std::endl;
                continue;
            }
            // 使用传统的for循环遍历content向量
            for (size_t paragraph_index = 0; paragraph_index < result[file_index].content.size(); ++paragraph_index) {
                output << result[file_index].content[paragraph_index];
            }
            output.close();
        }
    }
}

void FileManager::SaveTranslatedFiles(const std::vector<FileContent> &result, const std::filesystem::path &out_root_directory)
{
    // 创建输出目录
    if (!std::filesystem::exists(out_root_directory))
    {
        std::filesystem::create_directory(out_root_directory);
    }

    // 使用数组下标的方式遍历result并将内容保存至out_root_directory文件夹中
    for (size_t file_index = 0; file_index < result.size(); ++file_index)
    {
        std::filesystem::path out_path = out_root_directory / result[file_index].path;
        // 创建输出文件的目录
        if (!std::filesystem::exists(out_path.parent_path()))
        {
            std::filesystem::create_directories(out_path.parent_path());
        }
        std::ofstream output(out_path);
        if (!output.is_open())
        {
            std::cerr << "无法创建文件: " << out_path << std::endl;
            continue;
        }
        // 使用传统的for循环遍历content向量
        for (size_t paragraph_index = 0; paragraph_index < result[file_index].content.size(); ++paragraph_index)
        {
            if (paragraph_index % 2 != 0) // 检查paragraph_index是否为奇数
            {
                output << result[file_index].content[paragraph_index];
            }
        }
        output.close();
    }
}

void FileManager::CleanAll()
{
    // 清空翻译缓冲
    translation_cache.clear();
    m_file_index=0;
    m_paragraph_index = 0;
    m_cut_sign=false;
}

// 判断字符串末尾的文件类型
FileType FileManager::checkFileType(const std::string &filename)
{
    if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt") {
        return FileType::TXT;
    } else if (filename.size() > 3 && filename.substr(filename.size() - 3) == ".md") {
        return FileType::MD;
    } else if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".rst") {
        return FileType::RST;
    } else if (filename.size() > 2 && filename.substr(filename.size() - 2) == ".h") {
        return FileType::HPP;
    } else {
        return FileType::UNKNOWN;
    }
}
