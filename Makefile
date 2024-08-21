.PHONY : clean all run




gui:
	cd GUI/lvgl/ && cmake -B build && cmake --build build -j && cd -

llm:
	cd Server/llama.cpp/ && cmake -B build && cmake --build build -j && cd -

