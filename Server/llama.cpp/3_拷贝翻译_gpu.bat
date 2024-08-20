chcp 65001

.\build\bin\Release\copy_translator_llama_cpp.exe -m D:\models\qwen1_5-14b-chat-q2_k.gguf -f .\prompts\copy_translator.txt -t 16 --keep -1 -c 512 -b 512 -ub 512 -n -1 -s 5 -r "<|im_end|>" -ngl 16  --color -i --chatml 

CALL pause
