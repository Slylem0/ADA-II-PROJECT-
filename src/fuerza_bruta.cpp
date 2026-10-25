#include "fuerza_bruta.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "costo.h"

Resultado resolver_fuerza_bruta(const std::vector<Tarea>& tareas) {
    int n = (int)tareas.size();
    if (n > MAX_N_FB) {
        throw std::invalid_argument("fuerza bruta admite como máximo " +
                                    std::to_string(MAX_N_FB) + " tareas (n = " +
                                    std::to_string(n) + ")");
    }

    std::vector<int> orden(n);
    for (int i = 0; i < n; ++i) orden[i] = i + 1;

    Resultado mejor{orden, calcular_costo(tareas, orden)};
    while (std::next_permutation(orden.begin(), orden.end())) {
        long long costo = calcular_costo(tareas, orden);
        if (costo < mejor.costo) mejor = {orden, costo};
    }
    return mejor;
}
