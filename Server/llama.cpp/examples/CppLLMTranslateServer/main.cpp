/*
 * @Date: 2024-08-20 15:11:12
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-21 08:45:39
 * @FilePath: /CppLLMTranslate/Server/llama.cpp/examples/CppLLMTranslateServer/main.cpp
 */
#include <iostream>
#include <thread>

#include "LLM_main.h"
#include "UDP_Server.h"

using namespace std;

void UDP_llm_main_thread(int argc, char **argv)
{
    llm_main(argc, argv);

    while (1) {
        ;
    }
}

void UDP_Server_thread(int port)
{
    UDP_Server server(port);

    if (server.Initialize()) {
        server.Run();
    }

    while (1) {
        ;
    }
}

int main(int argc, char **argv)
{
    thread t_llm_main_thread(UDP_llm_main_thread, argc, argv);

    thread t_UDP_Server_thread(UDP_Server_thread, 59218);

    t_llm_main_thread.detach();
    t_UDP_Server_thread.detach();

    while (1) {
        ;
    }

    return 0;
}