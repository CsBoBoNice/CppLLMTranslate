/*
 * @Date: 2024-08-23 10:21:14
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-23 10:28:10
 * @FilePath: /lvgl/main/src/clipboard_text.cpp
 */
#include "clipboard_text.h"

#include <string.h>

std::mutex ClipboardSingleton::mutex;

ClipboardSingleton &ClipboardSingleton::getInstance()
{
    static ClipboardSingleton instance;
    return instance;
}

ClipboardSingleton::ClipboardSingleton() {}

void ClipboardSingleton::setCopyString(const std::string str)
{
    std::lock_guard<std::mutex> lock(mutex);
    copy_string = str;
}

std::string ClipboardSingleton::getCopyString()
{
    std::lock_guard<std::mutex> lock(mutex);
    return copy_string;
}

int ClipboardSingleton::run()
{
    // 用于存储上一次剪贴板内容的变量
    char *last_clipboard_text = NULL;

    while (1) {
        // 1. 获取剪贴板数据并打印
        char *clipboard_text = SDL_GetClipboardText();
        if (clipboard_text != NULL) {
            if (last_clipboard_text == NULL || strcmp(clipboard_text, last_clipboard_text) != 0) {
                // 打印剪贴板内容
                printf("剪贴板内容: %s\n", clipboard_text);

                last_clipboard_text = clipboard_text; // 更新上一次剪贴板内容的变量
                setCopyString(clipboard_text);        // 将剪贴板内容存储到全局变量中
            }
        } else {
            // printf("剪贴板内容为空\n");
        }
        SDL_Delay(100); // 等待一段时间后再次检查剪贴板内容
    }

    return 0;
}
