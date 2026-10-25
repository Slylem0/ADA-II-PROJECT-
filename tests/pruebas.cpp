// Pruebas automaticas del proyecto (sin librerias externas).
// Ejecutar con: make test

#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "costo.h"
#include "dp.h"
#include "entrada.h"
#include "fuerza_bruta.h"
#include "tarea.h"
#include "voraz.h"

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

// Instancias aleatorias pequenas, siempre las mismas (semilla fija).
static std::vector<std::vector<Tarea>> instancias_aleatorias() {
    std::mt19937 gen(12345);
    std::vector<std::vector<Tarea>> lista;
    for (int k = 0; k < 300; ++k) {
        int n = std::uniform_int_distribution<int>(1, 8)(gen);
        std::vector<Tarea> t;
        long long suma = 0;
        for (int i = 1; i <= n; ++i) {
            long long p = std::uniform_int_distribution<long long>(1, 10)(gen);
            long long w = std::uniform_int_distribution<long long>(1, 10)(gen);
            t.push_back({i, p, 0, w});
            suma += p;
        }
        for (Tarea& x : t) x.d = std::uniform_int_distribution<long long>(0, suma)(gen);
        lista.push_back(t);
    }
    return lista;
}

// El orden es una permutacion de 1..n y su costo coincide con el reportado.
static bool resultado_valido(const std::vector<Tarea>& t, const Resultado& r) {
    std::vector<int> ids = r.orden;
    std::sort(ids.begin(), ids.end());
    for (size_t i = 0; i < ids.size(); ++i) {
        if (ids[i] != (int)i + 1) return false;
    }
    return ids.size() == t.size() && calcular_costo(t, r.orden) == r.costo;
}

static std::vector<Tarea> tareas_iguales(int n) {
    std::vector<Tarea> t;
    for (int i = 1; i <= n; ++i) t.push_back({i, 1, 0, 1});
    return t;
}

template <class F>
static bool lanza_invalid_argument(F f) {
    try {
        f();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

static void prueba_dp_ejemplo() {
    auto t = ejemplo_abc();
    Resultado dp = resolver_dp(t);
    Resultado fb = resolver_fuerza_bruta(t);
    CHECK(dp.costo == 5);
    CHECK(fb.costo == 5);
    CHECK(resultado_valido(t, dp));
    CHECK(resultado_valido(t, fb));
}

static void prueba_dp_igual_fb() {
    for (const auto& t : instancias_aleatorias()) {
        Resultado dp = resolver_dp(t);
        CHECK(dp.costo == resolver_fuerza_bruta(t).costo);
        CHECK(resultado_valido(t, dp));
    }
}

static void prueba_limites() {
    CHECK(lanza_invalid_argument([] { resolver_dp(tareas_iguales(MAX_N_DP + 1)); }));
    CHECK(lanza_invalid_argument([] { resolver_fuerza_bruta(tareas_iguales(MAX_N_FB + 1)); }));
    CHECK(resolver_dp({}).costo == 0);
    CHECK(resolver_fuerza_bruta({}).costo == 0);
    CHECK(resolver_dp({}).orden.empty());
}

static void prueba_edd_orden() {
    std::vector<Tarea> t = {{1, 1, 5, 1}, {2, 1, 2, 1}, {3, 1, 9, 1}};
    CHECK(resolver_edd(t).orden == std::vector<int>({2, 1, 3}));
}

static void prueba_wspt_orden() {
    std::vector<Tarea> t = {{1, 2, 0, 1}, {2, 1, 0, 3}, {3, 4, 0, 4}};
    CHECK(resolver_wspt(t).orden == std::vector<int>({2, 3, 1}));
}

static void prueba_wmdd_orden() {
    Resultado r = resolver_wmdd(ejemplo_abc());
    CHECK(r.orden == std::vector<int>({2, 1, 3}));
    CHECK(r.costo == 5);
}

static void prueba_voraz_cota() {
    for (const auto& t : instancias_aleatorias()) {
        long long optimo = resolver_dp(t).costo;
        for (const Resultado& r : {resolver_edd(t), resolver_wspt(t), resolver_wmdd(t)}) {
            CHECK(resultado_valido(t, r));
            CHECK(r.costo >= optimo);
        }
    }
}

static void prueba_voraz_existe_suboptimo() {
    bool encontrado = false;
    for (const auto& t : instancias_aleatorias()) {
        if (resolver_wmdd(t).costo > resolver_dp(t).costo) encontrado = true;
    }
    CHECK(encontrado);
}

static void prueba_pesos_cero() {
    std::vector<Tarea> t = {{1, 2, 0, 0}, {2, 3, 0, 0}, {3, 1, 0, 0}};
    CHECK(resolver_dp(t).costo == 0);
    CHECK(resolver_edd(t).costo == 0);
    CHECK(resolver_wspt(t).costo == 0);
    CHECK(resolver_wmdd(t).costo == 0);
    CHECK(resultado_valido(t, resolver_wmdd(t)));
}

static void prueba_valores_maximos() {
    std::vector<Tarea> t;
    for (int i = 1; i <= 1000; ++i) t.push_back({i, 1000000, 0, 1000000});
    const long long esperado = 1000000LL * 1000000LL * (1000LL * 1001 / 2);
    CHECK(resolver_wmdd(t).costo == esperado);
    CHECK(resolver_wspt(t).costo == esperado);
}

int main() {
    prueba_costo_ejemplo();
    prueba_detalle();
    prueba_lectura_valida();
    prueba_lectura_vacia();
    prueba_lectura_errores();
    prueba_dp_ejemplo();
    prueba_dp_igual_fb();
    prueba_limites();
    prueba_edd_orden();
    prueba_wspt_orden();
    prueba_wmdd_orden();
    prueba_voraz_cota();
    prueba_voraz_existe_suboptimo();
    prueba_pesos_cero();
    prueba_valores_maximos();

    if (fallos > 0) {
        std::cerr << fallos << " de " << comprobaciones << " comprobaciones fallaron\n";
        return 1;
    }
    std::cout << "OK (" << comprobaciones << " comprobaciones)\n";
    return 0;
}
