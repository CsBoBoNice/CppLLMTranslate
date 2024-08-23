.PHONY : clean all run




all:
	cmake -B build && bear -- cmake --build build -j

run:
	./bin/main

clean:
	rm -rf build && rm -rf bin
