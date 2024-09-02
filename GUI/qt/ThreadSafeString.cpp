#include "ThreadSafeString.h"

ThreadSafeString::ThreadSafeString() {}

ThreadSafeString::ThreadSafeString(const std::string &str)
{
    set(str);
}

ThreadSafeString::~ThreadSafeString() {}

void ThreadSafeString::set(const std::string &str)
{
    std::lock_guard<std::mutex> lock(mtx);
    data = str;
}

void ThreadSafeString::append(const std::string &str)
{
    std::lock_guard<std::mutex> lock(mtx);
    data += str;
}

std::string ThreadSafeString::get() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return data;
}
