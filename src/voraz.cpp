#include "voraz.h"

#include <algorithm>

#include "costo.h"

namespace {

// Ids 1..n en orden estable segun el criterio `antes(a, b)`.
template <class Criterio>
Resultado ordenar_por(const std::vector<Tarea>& tareas, Criterio antes) {
    std::vector<int> orden;
    for (const Tarea& t : tareas) orden.push_back(t.id);
    std::stable_sort(orden.begin(), orden.end(), [&](int a, int b) {
        return antes(tareas[a - 1], tareas[b - 1]);
    });
    return {orden, calcular_costo(tareas, orden)};
}

}  // namespace

Resultado resolver_edd(const std::vector<Tarea>& tareas) {
    return ordenar_por(tareas, [](const Tarea& a, const Tarea& b) { return a.d < b.d; });
}

Resultado resolver_wspt(const std::vector<Tarea>& tareas) {
    // w_a / p_a > w_b / p_b  <=>  w_a * p_b > w_b * p_a  (p > 0), sin usar flotantes.
    return ordenar_por(tareas,
                       [](const Tarea& a, const Tarea& b) { return a.w * b.p > b.w * a.p; });
}

Resultado resolver_wmdd(const std::vector<Tarea>& tareas) {
    int n = (int)tareas.size();
    std::vector<bool> usada(n, false);
    std::vector<int> orden;
    long long t = 0;

    for (int paso = 0; paso < n; ++paso) {
        int elegida = -1;
        long long mejor_a = 0, mejor_w = 0;  // prioridad de la elegida: mejor_a / mejor_w
        for (int j = 0; j < n; ++j) {
            if (usada[j]) continue;
            long long a = std::max(tareas[j].p, tareas[j].d - t);
            long long w = tareas[j].w;
            if (elegida == -1) {
                elegida = j, mejor_a = a, mejor_w = w;
                continue;
            }
            // w = 0 equivale a prioridad infinita (no cuesta nada retrasarla).
            bool mejor;
            if (w == 0) mejor = false;
            else if (mejor_w == 0) mejor = true;
            else mejor = a * mejor_w < mejor_a * w;  // a/w < mejor_a/mejor_w
            if (mejor) elegida = j, mejor_a = a, mejor_w = w;
        }
        usada[elegida] = true;
        orden.push_back(tareas[elegida].id);
        t += tareas[elegida].p;
    }
    return {orden, calcular_costo(tareas, orden)};
}
