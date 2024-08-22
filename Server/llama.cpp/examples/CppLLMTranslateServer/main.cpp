/*
 * @Date: 2024-08-20 15:11:12
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-22 13:57:32
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

    server_p = new UDP_Server(59218);
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