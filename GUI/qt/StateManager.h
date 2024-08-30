/*
 * @Date: 2024-08-26 14:25:55
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-30 13:41:54
 * @FilePath: /CppLLMTranslate/GUI/qt/StateManager.h
 */
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <string>
#include <mutex>
#include <iostream>
#include "UDP_Client.h"

class StateManager {
  public:
    // 获取单例实例的静态方法
    static StateManager &getInstance();

    UDP_Client *client_p = nullptr;

    int ShowPage = 0; // 显示哪一个页面

  private:
    // 私有构造函数和析构函数
    StateManager() {}
    ~StateManager() {}

    // 静态实例指针
    static StateManager *instance;

    static std::mutex staticMutex; // 声明静态互斥量

  private:
};

#endif // STATEMANAGER_H
