/*
 * @Date: 2024-08-26 14:25:55
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-26 14:29:00
 * @FilePath: /GUI/qt/StateManager.cpp
 */
#include "StateManager.h"

// 初始化静态实例指针
StateManager *StateManager::instance = nullptr;

// 定义一个静态互斥量用于双重检查锁定
std::mutex StateManager::staticMutex;

StateManager &StateManager::getInstance()
{
    // 双重检查锁定，确保线程安全
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(staticMutex);
        if (instance == nullptr) {
            instance = new StateManager();
        }
    }
    return *instance;
}
