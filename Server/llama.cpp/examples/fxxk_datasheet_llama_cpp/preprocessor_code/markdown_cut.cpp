#include <iostream>
#include <fstream>
#include <string>

// 检查行是否是标题的函数
bool isTitle(const std::string& line) {
    size_t firstHash = line.find('#');
    return firstHash == 0 || (firstHash != std::string::npos && firstHash == line.find_first_not_of(' ') && line[firstHash + 1] == ' ');
}

// 处理文件的函数
void processMarkdownFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "无法打开文件。" << std::endl;
        return;
    }

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

int main() {
    std::string inputFilePath = "D:/code/markdown_cut/markdown_cut/input.md";
    std::string outputFilePath = "D:/code/markdown_cut/markdown_cut/output.md";

    processMarkdownFile(inputFilePath, outputFilePath);

    std::cout << "处理完成。" << std::endl;
    return 0;
}
