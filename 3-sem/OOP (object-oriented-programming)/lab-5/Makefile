.PHONY: build run tests allocator-tests queue-tests

build:
	mkdir -p build
	cd ./build; cmake ..; make all

run:
	./build/*_exe

tests:
	./build/*_allocator_tests
	./build/*_queue_tests

allocator-tests:
	./build/*_allocator_tests

queue-tests:
	./build/*_queue_tests
