.PHONY: build run tests point-tests rectangle-tests square-tests trapeze-tests vector-tests clean

build:
	mkdir -p build
	cd ./build; cmake ..; make all

run:
	./build/*_exe

tests:
	./build/*_point_tests
	./build/*_rectangle_tests
	./build/*_square_tests
	./build/*_trapeze_tests
	./build/*_vector_tests

point-tests:
	./build/*_point_tests

rectangle-tests:
	./build/*_rectangle_tests

square-tests:
	./build/*_square_tests

trapeze-tests:
	./build/*_trapeze_tests

vector-tests:
	./build/*_vector_tests

clean:
	rm -rf ./build/