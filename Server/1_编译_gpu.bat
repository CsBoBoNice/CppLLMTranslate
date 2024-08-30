
chcp 65001



CALL C:\ProgramData\anaconda3\Scripts\activate.bat C:\Users\75154\.conda\envs\llama_cpp

CALL cd llama.cpp && cmake -B build_gpu -DGGML_CUDA=ON && cmake --build build_gpu --config Release -j

CALL pause
