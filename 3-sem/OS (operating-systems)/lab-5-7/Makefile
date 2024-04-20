.PHONY: build run clean strace

build:
	mkdir bin
	g++ run.cpp -o ./server -lzmq
	g++ node.cpp -o ./node -lzmq

run:
	./server

clean:
	rm -rf ./server
	rm -rf ./node

strace:
	strace -f ./server