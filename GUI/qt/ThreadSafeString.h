/*
 * @Date: 2024-09-02 15:23:40
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-02 15:51:31
 * @FilePath: /CppLLMTranslate/GUI/qt/ThreadSafeString.h
 */
#ifndef THREADSAFESTRING_H
#define THREADSAFESTRING_H

#include <string>
#include <mutex>

class ThreadSafeString {
  public:
    ThreadSafeString();
    ThreadSafeString(const std::string &str);
    ~ThreadSafeString();

    // 禁止拷贝构造和赋值操作
    ThreadSafeString(const ThreadSafeString &) = delete;
    ThreadSafeString &operator=(const ThreadSafeString &) = delete;

    // 设置内容
    void set(const std::string &str);

    // 追加内容
    void append(const std::string &str);

    // 获取内容
    std::string get() const;

  private:
    mutable std::mutex mtx; // 用于保护string的互斥量
    std::string data;       // 存储字符串数据
};

#endif // THREADSAFESTRING_H
