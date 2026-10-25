#ifndef GENERACION_H
#define GENERACION_H

#include <ostream>
#include <vector>

#include "tarea.h"

// Que tan holgadas son las fechas limite: d se elige uniforme en [0, factor * suma de p].
// Alta = 1.0, Media = 0.6, Baja = 0.3 (con holgura baja casi todas las tareas llegan tarde).
enum class Holgura { Alta, Media, Baja };

// Instancia aleatoria reproducible: la misma (n, semilla, holgura) da siempre las mismas
// tareas. p y w uniformes en [1, 10].
std::vector<Tarea> generar_instancia(int n, unsigned semilla, Holgura holgura);

// Escribe las tareas en el formato de entrada que acepta leer_instancia.
void escribir_instancia(std::ostream& out, const std::vector<Tarea>& tareas);

#endif
