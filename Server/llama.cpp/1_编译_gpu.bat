
chcp 65001



CALL C:\ProgramData\anaconda3\Scripts\activate.bat C:\Users\75154\.conda\envs\llama_cpp

CALL cmake -B build -DGGML_CUDA=ON && cmake --build build --config Release -j

CALL pause
