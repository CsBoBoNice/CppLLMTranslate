#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

// 函数声明
void ProcessFiles(const std::filesystem::path& directory, const std::filesystem::path& target_directory, const std::filesystem::path& root_directory);

int main() {
    std::filesystem::path docs_path = "D:/code/file_cpy/file_cpy/docs";
    std::filesystem::path docs_en_path = "D:/code/file_cpy/file_cpy/docs_en";

    // 创建目标文件夹
    if (!std::filesystem::exists(docs_en_path)) {
        std::filesystem::create_directory(docs_en_path);
    }

    // 开始处理文件
    ProcessFiles(docs_path, docs_en_path, docs_path);

    return 0;
}

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
                std::ifstream input(entry.path());
                if (!input.is_open()) {
                    std::cerr << "无法打开文件: " << entry.path() << std::endl;
                    continue;
                }

                // 确保目标文件的目录存在
                if (!std::filesystem::exists(target_path.parent_path())) {
                    std::filesystem::create_directories(target_path.parent_path());
                }

                std::ofstream output(target_path);
                if (!output.is_open()) {
                    std::cerr << "无法创建文件: " << target_path << std::endl;
                    input.close();
                    continue;
                }

                std::string line;
                while (std::getline(input, line)) {
                    output << line << "-------------" << std::endl;
                }

                input.close();
                output.close();
            }
            else {
                // 对于非.md文件，直接复制
                std::filesystem::copy_file(entry.path(), target_path, std::filesystem::copy_options::overwrite_existing);
            }
        }
    }
}
