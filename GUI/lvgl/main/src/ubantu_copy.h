/*
 * @Date: 2024-08-22 16:41:16
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-22 16:54:09
 * @FilePath: /server/src/ubantu_copy.h
 */
#ifndef CLIPBOARD_SINGLETON_H
#define CLIPBOARD_SINGLETON_H

#include <string>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

class ClipboardSingleton {
public:
    // 获取单例实例的静态方法
    static ClipboardSingleton& getInstance();

    // 设置剪贴板内容的线程安全方法
    void setCopyString(const std::string str);

    // 获取剪贴板内容的线程安全方法
    std::string getCopyString();

    // 启动剪贴板监控的函数
    int run();

private:
    // 私有构造函数，防止外部创建实例
    ClipboardSingleton();
    ClipboardSingleton(const ClipboardSingleton&) = delete;
    ClipboardSingleton& operator=(const ClipboardSingleton&) = delete;

    // 用于存储剪贴板内容的成员变量
    std::string copy_string;

    // 用于确保线程安全的互斥锁
    static std::mutex mutex;

    // 监控剪贴板的私有方法
    void monitorClipboard(Display *display, Window window);

    // X11 display 和 window 变量
    Display *display;
    Window window;
};

#endif // CLIPBOARD_SINGLETON_H
