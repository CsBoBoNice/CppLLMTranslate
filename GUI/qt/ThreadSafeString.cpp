/*
 * @Date: 2024-09-02 15:23:48
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-11 13:56:11
 * @FilePath: /CppLLMTranslate/GUI/qt/ThreadSafeString.cpp
 */
#include "ThreadSafeString.h"

ThreadSafeString::ThreadSafeString() {}

ThreadSafeString::ThreadSafeString(const std::string &str)
{
    Set(str);
}

ThreadSafeString::~ThreadSafeString() {}

void ThreadSafeString::Set(const std::string &str)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data = str;
}

void ThreadSafeString::Append(const std::string &str)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data += str;
}

std::string ThreadSafeString::Get() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data;
}
