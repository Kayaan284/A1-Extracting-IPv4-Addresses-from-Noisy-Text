CXX = c++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -Wconversion

.PHONY: all test clean
all: ipv4

ipv4: main.cpp ipv4.cpp ipv4.h
	$(CXX) $(CXXFLAGS) main.cpp ipv4.cpp -o ipv4

test_ipv4: tests/test_ipv4.cpp ipv4.cpp ipv4.h
	$(CXX) $(CXXFLAGS) -I. tests/test_ipv4.cpp ipv4.cpp -o test_ipv4

test: ipv4 test_ipv4
	./test_ipv4
	python3 tests/test_cli.py ./ipv4

clean:
	rm -f ipv4 test_ipv4
