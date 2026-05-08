create:
	@if [ -z "$(VCPKG_ROOT)" ]; then \
		echo "Error : VCPKG_ROOT is not defined"; \
		exit 1; \
	fi
	cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

build:
	cmake --build build

run:
	make build && ./build/koda

test:
	cmake --build build --target koda_tests && cd build && ctest --output-on-failure

.PHONY: create build run test