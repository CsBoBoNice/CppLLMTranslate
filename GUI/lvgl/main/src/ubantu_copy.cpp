#include "ubantu_copy.h"

#include <string.h>

std::mutex ClipboardSingleton::mutex;

ClipboardSingleton &ClipboardSingleton::getInstance()
{
    static ClipboardSingleton instance;
    return instance;
}

ClipboardSingleton::ClipboardSingleton() : display(nullptr), window(0) {}

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

void ClipboardSingleton::monitorClipboard(Display *display, Window window)
{
    // ... 保持原来的 monitor_clipboard 函数内容不变 ...
    // 注意：你需要将 set_copy_string(data) 替换为 this->setCopyString(data)
    // 并且确保使用 this->getCopyString() 来访问剪贴板内容

    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    Atom targets = XInternAtom(display, "TARGETS", False);

    // 请求剪贴板数据
    XConvertSelection(display, clipboard, utf8_string, utf8_string, window, CurrentTime);

    // 用于存储上一次剪贴板内容的变量
    unsigned char *last_data = NULL;

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == SelectionNotify && event.xselection.selection == clipboard) {
            if (event.xselection.property != None) {
                // 获取剪贴板内容
                Atom actual_type;
                int actual_format;
                unsigned long nitems, bytes_after;
                unsigned char *data = NULL;
                XGetWindowProperty(display, window, event.xselection.property, 0, 1024, False, utf8_string,
                                   &actual_type, &actual_format, &nitems, &bytes_after, &data);

                if (data != NULL) {
                    if (last_data == NULL || strcmp((char *)data, (char *)last_data) != 0) {
                        // 打印剪贴板内容
                        printf("剪贴板内容: %s\n", (char *)data);

                        XFree(last_data);
                        last_data = data;            // 更新上一次剪贴板内容的变量
                        setCopyString((char *)data); // 将剪贴板内容存储到全局变量中
                    }
                } else {
                    // printf("剪贴板内容为空\n");
                }

                // 重新请求剪贴板数据以持续监控
                XConvertSelection(display, clipboard, utf8_string, utf8_string, window, CurrentTime);
            } else {
                printf("无法获取剪贴板数据\n");
                break;
            }
        }
    }
}

int ClipboardSingleton::run()
{
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "无法打开X显示\n");
        exit(1);
    }

    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
    XMapWindow(display, window);

    monitorClipboard(display, window);

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
