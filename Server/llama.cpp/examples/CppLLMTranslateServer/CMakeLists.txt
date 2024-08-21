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
)
install(TARGETS ${TARGET} RUNTIME)
target_link_libraries(${TARGET} PRIVATE common llama ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(${TARGET} PRIVATE cxx_std_17)
