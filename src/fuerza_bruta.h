#ifndef FUERZA_BRUTA_H
#define FUERZA_BRUTA_H

#include <vector>

#include "tarea.h"

// Maximo n aceptado: n! permutaciones crece demasiado rapido.
constexpr int MAX_N_FB = 10;

// Prueba los n! ordenes posibles y devuelve el de menor costo.
// Tiempo O(n * n!), espacio O(n). Lanza std::invalid_argument si n > MAX_N_FB.
Resultado resolver_fuerza_bruta(const std::vector<Tarea>& tareas);

#endif
