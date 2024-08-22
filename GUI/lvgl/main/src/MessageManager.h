/*
 * @Date: 2024-08-21 09:08:21
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-22 14:53:27
 * @FilePath: /CppLLMTranslate/GUI/lvgl/main/src/MessageManager.h
 */
// MessageManager.h
#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <queue>
#include <string>
#include <mutex>
#include <memory>
#include <condition_variable>

class MessageManager {
  public:
    // 获取单例实例的静态方法
    static MessageManager &getInstance();

    // 防止拷贝构造和赋值操作
    MessageManager(const MessageManager &) = delete;
    MessageManager &operator=(const MessageManager &) = delete;

    // 将消息加入输入队列
    void pushToInputQueue(const std::string &message);
    // 从输入队列取出消息
    bool popFromInputQueue(std::string &message);
    // 从输入队列取出消息，不等待
    bool popFromInputQueueNoWait(std::string &message);

    // 将消息加入输出队列
    void pushToOutputQueue(const std::string &message);
    // 从输出队列取出消息
    bool popFromOutputQueue(std::string &message);
    // 从输出队列取出消息，不等待
    bool popFromOutputQueueNoWait(std::string &message);


  private:
    // 私有构造函数和析构函数
    MessageManager() {}
    ~MessageManager() {}

    // 输入和输出队列
    std::queue<std::string> inputQueue;
    std::queue<std::string> outputQueue;
    // 互斥量
    std::mutex inputMutex;
    std::mutex outputMutex;

    std::condition_variable inputCondVar;
    std::condition_variable outputCondVar;

    // 静态实例指针
    static MessageManager *instance;

    static std::mutex staticMutex; // 声明静态互斥量
};

#endif // MESSAGEMANAGER_H
