chcp 65001

.\build\bin\Release\copy_translator_llama_cpp.exe -m D:\models\qwen1_5-14b-chat-q8_0.gguf -f .\prompts\fxxk_zh.txt -t 16 --keep -1 -c 2048 -b 2048 -ub 2048 -n -1 -s 5 -r "<|im_end|>" -i --chatml 

CALL pause
