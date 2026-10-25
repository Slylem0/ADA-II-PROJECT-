// Pruebas automaticas del proyecto (sin librerias externas).
// Ejecutar con: make test

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "costo.h"
#include "entrada.h"
#include "tarea.h"

static int comprobaciones = 0;
static int fallos = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        ++comprobaciones;                                                    \
        if (!(cond)) {                                                       \
            ++fallos;                                                        \
            std::cerr << __FILE__ << ":" << __LINE__ << ": fallo: " #cond    \
                      << "\n";                                               \
        }                                                                    \
    } while (0)

// Verifica que leer el texto dado lance std::runtime_error.
static bool lectura_falla(const std::string& texto) {
    std::istringstream in(texto);
    try {
        leer_instancia(in);
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

// Ejemplo del informe: A=(3,4,2), B=(2,2,5), C=(4,6,1).
static std::vector<Tarea> ejemplo_abc() {
    return {{1, 3, 4, 2}, {2, 2, 2, 5}, {3, 4, 6, 1}};
}

static void prueba_costo_ejemplo() {
    auto t = ejemplo_abc();
    CHECK(calcular_costo(t, {1, 2, 3}) == 18);
    CHECK(calcular_costo(t, {2, 1, 3}) == 5);
}

static void prueba_detalle() {
    auto filas = detalle_orden(ejemplo_abc(), {2, 1, 3});
    CHECK(filas.size() == 3);
    CHECK(filas[0].id == 2 && filas[1].id == 1 && filas[2].id == 3);
    CHECK(filas[0].fin == 2 && filas[1].fin == 5 && filas[2].fin == 9);
    CHECK(filas[0].retraso == 0 && filas[1].retraso == 1 && filas[2].retraso == 3);
    CHECK(filas[0].penalizacion == 0 && filas[1].penalizacion == 2 &&
          filas[2].penalizacion == 3);
}

static void prueba_lectura_valida() {
    std::istringstream in("# comentario\n3\n3 4 2\r\n\n2 2 5  \n4 6 1\n");
    auto t = leer_instancia(in);
    CHECK(t.size() == 3);
    if (t.size() != 3) return;
    CHECK(t[0].id == 1 && t[0].p == 3 && t[0].d == 4 && t[0].w == 2);
    CHECK(t[1].id == 2 && t[1].p == 2 && t[1].d == 2 && t[1].w == 5);
    CHECK(t[2].id == 3 && t[2].p == 4 && t[2].d == 6 && t[2].w == 1);
}

static void prueba_lectura_vacia() {
    std::istringstream in("0\n");
    auto t = leer_instancia(in);
    CHECK(t.empty());
    CHECK(calcular_costo(t, {}) == 0);
}

static void prueba_lectura_errores() {
    CHECK(lectura_falla(""));
    CHECK(lectura_falla("2\n1 1 1\n"));
    CHECK(lectura_falla("1\n0 1 1\n"));
    CHECK(lectura_falla("1\n1 -1 1\n"));
    CHECK(lectura_falla("1\n1 1 x\n"));
    CHECK(lectura_falla("1\n1 1 1 7\n"));
    CHECK(lectura_falla("1001\n"));
}

int main() {
    prueba_costo_ejemplo();
    prueba_detalle();
    prueba_lectura_valida();
    prueba_lectura_vacia();
    prueba_lectura_errores();

    if (fallos > 0) {
        std::cerr << fallos << " de " << comprobaciones << " comprobaciones fallaron\n";
        return 1;
    }
    std::cout << "OK (" << comprobaciones << " comprobaciones)\n";
    return 0;
}
