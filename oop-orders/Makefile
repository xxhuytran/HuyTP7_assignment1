CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: orders

orders: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o orders

run: orders
	./orders

clean:
	rm -f orders

.PHONY: all run clean
