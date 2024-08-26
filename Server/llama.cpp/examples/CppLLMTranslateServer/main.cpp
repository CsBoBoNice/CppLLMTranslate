/*
 * @Date: 2024-08-20 15:11:12
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-26 13:54:49
 * @FilePath: /llama.cpp/examples/CppLLMTranslateServer/main.cpp
 */
#include <iostream>
#include <thread>

#include "LLM_main.h"
#include "UDP_Server.h"

using namespace std;

void UDP_llm_main_thread(int argc, char **argv)
{
    llm_main(argc, argv);
}

UDP_Server *server_p = nullptr;

void UDP_Server_Recv_thread()
{
    server_p->Recv_thread();
}

void UDP_Server_Send_thread()
{
    server_p->Send_thread();
}

int main(int argc, char **argv)
{
    thread t_llm_main_thread(UDP_llm_main_thread, argc, argv);

    int port = 59218; // 默认端口号
    // 遍历命令行参数
    for (int i = 1; i < argc; ++i) {
        // 检查当前参数是否是 "-s"
        if (std::string(argv[i]) == "--port") {
            // 确保 "-s" 后面有参数
            if (i + 1 < argc) {
                // 尝试将下一个参数转换为整数
                int num = std::atoi(argv[i + 1]);
                if (num < 0) {
                    std::cerr << "Error: '--port' must be followed by a non-negative number." << std::endl;
                    return 1;
                }
                port = num;
            } else {
                std::cerr << "Error: '--port' does not follow a number." << std::endl;
                return 1;
            }
        }
    }

    server_p = new UDP_Server(port);
    server_p->Initialize();
    thread t_UDP_Server_Recv_thread(UDP_Server_Recv_thread);
    thread t_UDP_Server_Send_thread(UDP_Server_Send_thread);

    t_llm_main_thread.detach();
    t_UDP_Server_Recv_thread.detach();
    t_UDP_Server_Send_thread.detach();

    while (1) {
        ;
    }

    return 0;
}