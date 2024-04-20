.PHONY: build run vector_tests twelve_tests clean

build:
	mkdir -p build
	cd ./build; cmake ..; make all

run:
	./build/*_exe

twelve-tests:
	./build/*_twelve_tests

vector-tests:
	./build/*_vector_tests

clean:
	rm -rf ./build/