.PHONY: build run clean strace generate-test show-graphic

build: clean
	mkdir build
	cd ./build; cmake ..; make all

run:
	./build/*_exe $(threads)

clean:
	rm -rf ./build/

strace:
	strace -f ./build/*_exe $(threads)

generate-test:
	python3 ./generators/generator.py

show-graphic:
	python3 ./analytics/draw_graphic.py