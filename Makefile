build:
	clang++ -Wall -Wpedantic -Wextra -fsanitize=address -g --std=c++20 -o nc *.cc */*.cc

run: build
	./nc ./test.nc
