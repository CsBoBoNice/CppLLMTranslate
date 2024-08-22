#include "TranslationManager.h"

TranslationManager::TranslationManager() {}

TranslationManager::~TranslationManager() {}

void TranslationManager::removeTrailingImEndTag(std::string &str,std::string str_remove) {
    // 查找"<|im_end|>"标签的位置
    size_t pos = str.rfind(str_remove);
    if (pos != std::string::npos) {
        // 如果找到了，就删除它
        str.erase(pos, str_remove.length());
    }
}

// 检查行是否是标题的函数
bool TranslationManager::isTitle(const std::string &line) {
    size_t firstHash = line.find('#');
    return firstHash == 0 ||
           (firstHash != std::string::npos && firstHash == line.find_first_not_of(' ') && line[firstHash + 1] == ' ');
}

// 递归遍历文件夹中的文件,按文件后最区分并按标题分割，非特定文件则按目录结构拷贝
// directory为输入文件夹 target_directory为输出文件夹 directory与root_directory相同
void TranslationManager::ProcessFilesCut(const std::filesystem::path &directory,
                                         const std::filesystem::path &target_directory,
                                         const std::filesystem::path &root_directory) {
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
FileContent TranslationManager::getFileContent(const std::filesystem::directory_entry &entry) {
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
        }
        else if (paragraph.size() >= paragraph_effective &&
                 ((line.empty() &&
                   paragraph.size() >= paragraph_min) ||
                  (paragraph[paragraph.size() - 1] == '\n' && paragraph[paragraph.size() - 2] == '.' &&
                   paragraph.size() >= paragraph_min) ||
                  (paragraph[paragraph.size() - 1] == '\n' && paragraph.size() >= paragraph_max)))
        {
            // 检查是否是英文句号后紧跟换行符 读取的字节数大于 paragraph_min 或者 大于 paragraph_max 以 回车结尾
            fileContent.content.push_back(paragraph); // 添加换行符
            paragraph.clear();

            line += "\n";
            paragraph += line;
        }
        else
        {
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
                paragraph + "\nLife is actually like the weather, with its sunny days, cloudy days, and occasional "
                            "rain showers. It's the natural order of things. Life isn't simple, but we should strive "
                            "to simplify it as much as possible.\n"); // 添加换行符
        }
    }

    input.close();
    return fileContent;
}

// 递归遍历文件夹中的文件,按.md区分文件 按分隔符与标题段长度划分段落 保存至std::vector<FileContent>& result
void TranslationManager::ProcessFilesRecursive(const std::filesystem::path &directory,
                                               const std::filesystem::path &target_directory,
                                               const std::filesystem::path &root_directory,
                                               std::vector<FileContent> &result) {

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

            if (entry.path().filename() != "CMakeLists.txt" && (entry.path().extension() == ".md" || entry.path().extension() == ".txt" ||
                                                                entry.path().extension() == ".rst" || entry.path().extension() == ".h"))
            {
                FileContent fileContent;
                fileContent = getFileContent(entry);
                fileContent.path = relative_path.string();

                if (fileContent.content.size() == 0) {
                    continue;
                }

                result.push_back(fileContent);
            }
            else
            {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path,
                                           std::filesystem::copy_options::overwrite_existing);
            }
        }
    }
}

// 读取已翻译内容 保存至std::vector<FileContent>& result
void TranslationManager::ReadTranslated(const std::filesystem::path &directory,
                                        const std::filesystem::path &root_directory, std::vector<FileContent> &result) {
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relative_path = std::filesystem::relative(entry.path(), root_directory);

        if (std::filesystem::is_directory(entry.status())) {
            // 递归处理子目录
            ReadTranslated(entry.path(), root_directory, result);
        } else {
            if (entry.path().filename() != "CMakeLists.txt" && (entry.path().extension() == ".md" || entry.path().extension() == ".txt" ||
                                                                entry.path().extension() == ".rst" || entry.path().extension() == ".h"))
            {
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
void TranslationManager::SaveToFilesystem(const std::vector<FileContent> &result,
                                          const std::filesystem::path &out_root_directory, size_t index)
{
    // 使用数组下标的方式遍历result并将内容保存至out_root_directory文件夹中
    for (size_t file_index = 0; file_index < result.size(); ++file_index)
    {
        std::filesystem::path out_path = out_root_directory / result[file_index].path;
        // 创建输出文件的目录
        if (!std::filesystem::exists(out_path.parent_path()))
        {
            std::filesystem::create_directories(out_path.parent_path());
        }

        if (index == file_index)
        {
            std::ofstream output(out_path);
            if (!output.is_open())
            {
                std::cerr << "无法创建文件: " << out_path << std::endl;
                continue;
            }
            // 使用传统的for循环遍历content向量
            for (size_t paragraph_index = 0; paragraph_index < result[file_index].content.size(); ++paragraph_index)
            {
                output << result[file_index].content[paragraph_index];
            }
            output.close();
        }
    }
}

// 函数用于检查文件是否存在
bool TranslationManager::fileExists(const std::string &filename) {
    std::filesystem::path path(filename);
    return std::filesystem::exists(path);
}

// 将数据写入指定的json文件
void TranslationManager::writeJsonToFile(const std::string &filename, size_t file_index, size_t paragraph_index) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "file_index", file_index);
    cJSON_AddNumberToObject(root, "paragraph_index", paragraph_index);

    char *json_str = cJSON_Print(root);
    std::ofstream out(filename);
    if (out.is_open()) {
        out << json_str;
        out.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    cJSON_Delete(root);
    free(json_str);
}

// 从指定的json文件读取数据并打印到控制台
bool TranslationManager::readJsonFromFile(const std::string &filename, size_t &file_index, size_t &paragraph_index) {
    if (!fileExists(filename)) {
        file_index = 0;
        paragraph_index = 0;
        return false;
    }

    std::ifstream in(filename);
    if (in.is_open()) {
        std::string json_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        cJSON *root = cJSON_Parse(json_str.c_str());
        if (root != nullptr) {
            cJSON *file_index_json = cJSON_GetObjectItem(root, "file_index");
            cJSON *paragraph_index_json = cJSON_GetObjectItem(root, "paragraph_index");

            if (file_index_json != nullptr && paragraph_index_json != nullptr) {
                file_index = file_index_json->valueint;
                paragraph_index = paragraph_index_json->valueint;
                std::cout << "file_index: " << file_index << std::endl;
                std::cout << "paragraph_index: " << paragraph_index << std::endl;
            }

            cJSON_Delete(root);
        } else {
            std::cerr << "Failed to parse JSON." << std::endl;
        }

        in.close();
    } else {
        std::cerr << "Unable to open file for reading." << std::endl;
    }

    return true;
}

// 从指定的json文件读取段落分割数据
bool TranslationManager::readJsonFromFile_paragraph(const std::string &filename) {
    if (!fileExists(filename)) {
        printf("ERROR readJsonFromFile_paragraph !!!\n");
        return false;
    }

    std::ifstream in(filename);
    if (in.is_open()) {
        std::string json_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        cJSON *root = cJSON_Parse(json_str.c_str());
        if (root != nullptr) {

            cJSON *paragraph_effective_json = cJSON_GetObjectItem(root, "paragraph_effective");
            cJSON *paragraph_min_json = cJSON_GetObjectItem(root, "paragraph_min");
            cJSON *paragraph_max_json = cJSON_GetObjectItem(root, "paragraph_max");

            if (paragraph_effective_json != nullptr && paragraph_min_json != nullptr && paragraph_max_json != nullptr) {
                paragraph_effective = paragraph_effective_json->valueint;
                paragraph_min = paragraph_min_json->valueint;
                paragraph_max = paragraph_max_json->valueint;
                std::cout << "paragraph_effective: " << paragraph_effective << std::endl;
                std::cout << "paragraph_min: " << paragraph_min << std::endl;
                std::cout << "paragraph_max: " << paragraph_max << std::endl;
            }

            cJSON_Delete(root);
        } else {
            std::cerr << "Failed to parse JSON." << std::endl;
        }

        in.close();
    } else {
        std::cerr << "Unable to open file for reading." << std::endl;
    }

    return true;
}

// 读取指定文件通过string返回
std::string TranslationManager::readFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        std::terminate();  // 终止程序的运行
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 初始化翻译配置,包括优质回答等
void TranslationManager::init_systemInfo(void) {

    printf("\n init_systemInfo start \n");

    string_nice_input_rst_1 = readFile(nice_input_rst_1);
    string_nice_output_rst_1 = readFile(nice_output_rst_1);
    string_nice_input_rst_2 = readFile(nice_input_rst_2);
    string_nice_output_rst_2 = readFile(nice_output_rst_2);
    string_nice_input_rst_3 = readFile(nice_input_rst_3);
    string_nice_output_rst_3 = readFile(nice_output_rst_3);
    string_chat_prefix_rst = readFile(chat_prefix_rst);
    string_chat_suffix_rst = readFile(chat_suffix_rst);
    string_systemInfo_rst = readFile(systemInfo_rst);

    string_nice_input_md = readFile(nice_input_md);
    string_nice_output_md = readFile(nice_output_md);
    string_chat_prefix_md = readFile(chat_prefix_md);
    string_chat_suffix_md = readFile(chat_suffix_md);
    string_systemInfo_md = readFile(systemInfo_md);

    string_nice_input_hpp = readFile(nice_input_hpp);
    string_nice_output_hpp = readFile(nice_output_hpp);
    string_chat_prefix_hpp = readFile(chat_prefix_hpp);
    string_chat_suffix_hpp = readFile(chat_suffix_hpp);
    string_systemInfo_hpp = readFile(systemInfo_hpp);

    string_nice_input_chat = readFile(nice_input_chat);
    string_nice_output_chat = readFile(nice_output_chat);
    string_chat_prefix_chat = readFile(chat_prefix_chat);
    string_chat_suffix_chat = readFile(chat_suffix_chat);
    string_systemInfo_chat = readFile(systemInfo_chat);

    printf("\n init_systemInfo success \n");
}

// 函数用于判断字符串末尾的文件类型
FileType TranslationManager::checkFileType(const std::string &filename) {
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
