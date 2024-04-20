all: create-build exe-runtime exe-comptime

exe-runtime: ./third_party/prime_numbers/include/prime_count.h ./third_party/eulers_number/include/calculate_eulers_num.h run.cpp lib_eulers lib_prime
	g++ run_runtime.cpp -fPIC -L./build -ldl -o ./build/run_runtime

exe-comptime: ./third_party/prime_numbers/include/prime_count.h ./third_party/eulers_number/include/calculate_eulers_num.h run.cpp lib_eulers lib_prime
	g++ run.cpp -fPIC -L./build -leulers -lprime_numbers -o ./build/run

lib_eulers: ./third_party/eulers_number/include/calculate_eulers_num.h ./third_party/eulers_number/src/calculate_eulers_num.cpp 
	g++ -shared ./third_party/eulers_number/src/calculate_eulers_num.cpp -fPIC -o ./build/libeulers.so

lib_prime: ./third_party/prime_numbers/include/prime_count.h ./third_party/prime_numbers/src/prime_count.cpp
	g++ -shared ./third_party/prime_numbers/src/prime_count.cpp -fPIC -o ./build/libprime_numbers.so

create-build:
	mkdir -p ./build

run-runtime:
	cd build; ./run_runtime $(mode) $(a) $(b)=0

run-comptime:
	cd build; ./run $(mode) $(a) $(b)=0

strace-runtime:
	cd build; strace -f ./run_runtime $(mode) $(a) $(b)=0

strace-comptime:
	cd build; strace -f ./run $(mode) $(a) $(b)=0

clean:
	rm -rf build		