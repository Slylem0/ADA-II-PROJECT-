#include "dp.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

Resultado resolver_dp(const std::vector<Tarea>& tareas) {
    int n = (int)tareas.size();
    if (n > MAX_N_DP) {
        throw std::invalid_argument("programación dinámica admite como máximo " +
                                    std::to_string(MAX_N_DP) + " tareas (n = " +
                                    std::to_string(n) + ")");
    }

    // Los subconjuntos se representan como mascaras de bits: el bit j indica la tarea j.
    uint32_t total = 1u << n;
    std::vector<long long> f(total, std::numeric_limits<long long>::max());
    std::vector<int8_t> ultima(total, -1);  // tarea que va de ultima en el optimo de S
    f[0] = 0;

    // Todo subconjunto de S es numericamente menor que S, asi que al llegar a S
    // los valores f(S \ {j}) ya estan calculados.
    for (uint32_t S = 1; S < total; ++S) {
        // Las tareas de S, en cualquier orden, terminan juntas en el instante P(S).
        long long P = 0;
        for (int j = 0; j < n; ++j) {
            if (S & (1u << j)) P += tareas[j].p;
        }
        for (int j = 0; j < n; ++j) {
            if (!(S & (1u << j))) continue;
            long long costo = f[S ^ (1u << j)] + tareas[j].w * std::max(0LL, P - tareas[j].d);
            if (costo < f[S]) {
                f[S] = costo;
                ultima[S] = (int8_t)j;
            }
        }
    }

    // Reconstruccion: se retira la ultima tarea de S hasta vaciarlo.
    std::vector<int> orden;
    for (uint32_t S = total - 1; S != 0; S ^= 1u << ultima[S]) {
        orden.push_back(tareas[ultima[S]].id);
    }
    std::reverse(orden.begin(), orden.end());
    return {orden, f[total - 1]};
}
