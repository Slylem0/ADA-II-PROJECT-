#ifndef DP_H
#define DP_H

#include <vector>

#include "tarea.h"

// Maximo n aceptado: la tabla tiene 2^n estados (~9 bytes cada uno, ~150 MB para n = 24).
constexpr int MAX_N_DP = 24;

// Programacion dinamica sobre subconjuntos (solucion exacta).
//   f(vacio) = 0
//   f(S) = min_{j en S} f(S \ {j}) + w_j * max(0, P(S) - d_j),  P(S) = suma de p en S
// Tiempo O(2^n * n), espacio O(2^n). Lanza std::invalid_argument si n > MAX_N_DP.
Resultado resolver_dp(const std::vector<Tarea>& tareas);

#endif
