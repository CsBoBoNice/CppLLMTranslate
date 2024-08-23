set(TARGET CppLLMTranslateServer)
add_executable(${TARGET} main.cpp 
                            LLM_main.cpp 
                            LLM_main.h 
                            TranslationManager.cpp 
                            TranslationManager.h 
                            cJSON.c 
                            cJSON.h
                            UDP_Server.cpp
                            UDP_Server.h
                            MessageManager.cpp
                            MessageManager.h
                            agreement.cpp
                            agreement.h
)
install(TARGETS ${TARGET} RUNTIME)
target_link_libraries(${TARGET} PRIVATE common llama ${CMAKE_THREAD_LIBS_INIT})

# 使用 if() 语句来判断当前是否为 Windows 系统
if(WIN32)
    # 如果是在 Windows 系统上，链接 ws2_32
    target_link_libraries(${TARGET} PRIVATE ws2_32)

else()
    # 如果不是 Windows 系统，可能需要链接其他库或设置其他链接选项
    # 例如，在 Unix-like 系统上，你可能需要链接其他的网络库
    # target_link_libraries(udp_server PRIVATE net)
endif()


target_compile_features(${TARGET} PRIVATE cxx_std_17)
