/*
 * @Date: 2024-08-21 09:10:44
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-21 10:23:43
 * @FilePath: /CppLLMTranslate/Server/llama.cpp/examples/CppLLMTranslateServer/MessageManager.cpp
 */
// MessageManager.cpp
#include "MessageManager.h"

// 初始化静态实例指针
MessageManager *MessageManager::instance = nullptr;

// 定义一个静态互斥量用于双重检查锁定
std::mutex MessageManager::staticMutex;

MessageManager &MessageManager::getInstance()
{
    // 双重检查锁定，确保线程安全
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(staticMutex);
        if (instance == nullptr) {
            instance = new MessageManager();
        }
    }
    return *instance;
}

void MessageManager::pushToInputQueue(const std::string &message)
{
    std::lock_guard<std::mutex> lock(inputMutex);
    inputQueue.push(message);
    inputCondVar.notify_one(); // 通知可能有等待的线程
}

bool MessageManager::popFromInputQueue(std::string &message)
{
    std::unique_lock<std::mutex> lock(inputMutex);
    inputCondVar.wait(lock, [this] { return !inputQueue.empty(); });
    if (inputQueue.empty()) {
        return false;
    }
    message = inputQueue.front();
    inputQueue.pop();
    return true;
}

void MessageManager::pushToOutputQueue(const std::string &message)
{
    std::lock_guard<std::mutex> lock(outputMutex);
    outputQueue.push(message);
    outputCondVar.notify_one(); // 通知可能有等待的线程
}

bool MessageManager::popFromOutputQueue(std::string &message)
{
    std::unique_lock<std::mutex> lock(outputMutex);
    outputCondVar.wait(lock, [this] { return !outputQueue.empty(); });
    if (outputQueue.empty()) {
        return false;
    }
    message = outputQueue.front();
    outputQueue.pop();
    return true;
}