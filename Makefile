all: 
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON 
	cmake --build ./build 
clean:
	cd build && make clean
