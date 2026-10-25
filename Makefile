CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Isrc

# Modulos compartidos por todos los ejecutables
LIB_SRCS = src/entrada.cpp src/costo.cpp src/fuerza_bruta.cpp src/dp.cpp src/voraz.cpp src/generacion.cpp
LIB_OBJS = $(LIB_SRCS:src/%.cpp=build/%.o)
HEADERS  = $(wildcard src/*.h)

.PHONY: all test clean

all: tareas generador experimentos pruebas

build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

pruebas: tests/pruebas.cpp $(LIB_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) tests/pruebas.cpp $(LIB_OBJS) -o $@

tareas: src/main.cpp $(LIB_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(LIB_OBJS) -o $@

generador: src/generador.cpp $(LIB_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/generador.cpp $(LIB_OBJS) -o $@

experimentos: src/experimentos.cpp $(LIB_OBJS) $(HEADERS)
	$(CXX) $(CXXFLAGS) src/experimentos.cpp $(LIB_OBJS) -o $@

test: pruebas tareas generador
	./pruebas
	@# Pruebas del ejecutable tareas
	./tareas ejemplos/ejemplo_basico.txt --algoritmo dp | grep -q "Costo total: 5"
	./tareas --algoritmo dp < ejemplos/voraz_no_optimo.txt | grep -q "Costo total: 55"
	printf '2\n1 1 1\n' | ./tareas > /dev/null 2>&1; test $$? -eq 1
	./tareas no_existe.txt > /dev/null 2>&1; test $$? -eq 1
	./tareas ejemplos/ejemplo_basico.txt --opcion-rara > /dev/null 2>&1; test $$? -eq 1
	{ echo 25; for i in $$(seq 25); do echo "1 0 1"; done; } | ./tareas --algoritmo dp > /dev/null 2>&1; test $$? -eq 1
	{ echo 25; for i in $$(seq 25); do echo "1 0 1"; done; } | ./tareas | grep -q "Costo total"
	./generador 6 3 --holgura baja | ./tareas --algoritmo dp | grep -q "Costo total"
	./generador 6 > /dev/null 2>&1; test $$? -eq 1
	@echo "OK (pruebas del ejecutable)"

clean:
	rm -rf build pruebas tareas generador experimentos
