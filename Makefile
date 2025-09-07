clean:
	@rm -rf build
	@echo "Build folder cleaned"

build_debug:
	@echo "Building DEBUG version"
	@cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build build

build_release:
	@echo "Building RELEASE version"
	@cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build build

run_proj:
	@./build/pathFinder

test_proj:
	@ctest --test-dir build

.PHONY: clean build_debug build_release run_proj