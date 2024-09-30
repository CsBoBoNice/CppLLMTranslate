
#ifndef MODELSINFO_H
#define MODELSINFO_H

#include <string>
#include <QString>

struct ServerInfo {
    std::string url;
    std::string apiKey;
    std::string model;
};

class ModelsInfo {
  public:
    // 获取单例对象的静态方法
    static ModelsInfo &getInstance();

    // 禁止拷贝构造和赋值操作
    ModelsInfo(const ModelsInfo &) = delete;
    ModelsInfo &operator=(const ModelsInfo &) = delete;

    // 读取文件内容并返回 std::string
    std::string readFile(const QString &filePath);

    // 将字符串内容保存到文件
    bool saveFile(const QString &filePath, const std::string &content);

    ServerInfo DefaultGetServerIP();
    ServerInfo GetServerIP();
    void SetServerIP(const ServerInfo &newServerInfo);

  private:
    // 私有构造函数
    ModelsInfo() = default;
};

#endif // MODELSINFO_H