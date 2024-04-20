.PHONY: build

build: clean
	mkdir build
	cd ./build; cmake ..; make all

run:
	./build/src/*_run_exe
	
clean:
	rm -rf ./build/

strace:
	strace -f ./build/src/*_run_exe

unlink-shm:
	rm /dev/shm/*