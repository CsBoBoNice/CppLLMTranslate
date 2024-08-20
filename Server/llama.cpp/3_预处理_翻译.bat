chcp 65001

.\build\bin\Release\fxxk_processing_datasheet_llama_cpp.exe -m D:\models\qwen1_5-14b-chat-q8_0.gguf  -f .\prompts\fxxk_processing_zh.txt -t 16 --keep -1 -c 4096 -b 4096 -ub 4096 -n -1 -s 5 -r "<|im_end|>" -i --chatml 

.\build\bin\Release\fxxk_datasheet_llama_cpp.exe -m D:\models\dolphin-2.9.1-yi-1.5-9b-Q4_K_S.gguf -f .\prompts\fxxk_zh.txt -t 16 --keep -1 -c 4096 -b 4096 -ub 4096 -n -1 -s 5 -r "<|im_end|>" -i --chatml 

CALL pause
