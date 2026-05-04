create:
	cmake -B build/ -S .

build: 
	cmake --build build

run:
	make build && ./build/koda

.PHONY: create build run