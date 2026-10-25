#ifndef VORAZ_H
#define VORAZ_H

#include <vector>

#include "tarea.h"

// Algoritmos voraces (heuristicos): rapidos pero sin garantia de optimalidad.
// En todos, los empates se resuelven a favor del id menor.

// EDD (Earliest Due Date): ordena por fecha limite d creciente. O(n log n).
Resultado resolver_edd(const std::vector<Tarea>& tareas);

// WSPT (regla de Smith): ordena por w/p decreciente. O(n log n).
Resultado resolver_wspt(const std::vector<Tarea>& tareas);

// WMDD (Weighted Modified Due Date): en cada paso, con tiempo actual t, elige la
// tarea pendiente que minimiza max(p, d - t) / w. Las de w = 0 van al final. O(n^2).
Resultado resolver_wmdd(const std::vector<Tarea>& tareas);

#endif
