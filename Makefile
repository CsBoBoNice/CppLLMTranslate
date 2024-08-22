.PHONY : clean all run




gui:
	cd GUI/lvgl/ && cmake -B build && cmake --build build -j && cd -

llm:
	cd Server/llama.cpp/ && cmake -B build && cmake --build build -j && cd -


llm_run:
	cd Server/llama.cpp/build && ./build/bin/CppLLMTranslateServer -m ~/models/gemma-2-2B-it-Q8_0.gguf -p "hi" -t 4 -c 8192 -n -1 -s 555 --temp 0 --color -cnv --no-escape

gui_run:
	cd GUI/lvgl/build && ./build/bin/main