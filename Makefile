CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Isrc

# Modulos compartidos por todos los ejecutables
LIB_SRCS = src/entrada.cpp src/costo.cpp src/fuerza_bruta.cpp src/dp.cpp
LIB_OBJS = $(LIB_SRCS:src/%.cpp=build/%.o)
HEADERS  = $(wildcard src/*.h)

.PHONY: all test clean

all: pruebas

build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

pruebas: tests/pruebas.cpp $(LIB_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) tests/pruebas.cpp $(LIB_OBJS) -o $@

test: pruebas
	./pruebas

clean:
	rm -rf build pruebas
