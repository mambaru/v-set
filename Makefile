all: 
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON 
	cmake --build ./build 
	doxygen
clean:
	cd build && make clean
