#ifndef COSTO_H
#define COSTO_H

#include <vector>

#include "tarea.h"

// Costo total de ejecutar las tareas en `orden` (ids): suma de w_j * max(0, C_j - d_j).
long long calcular_costo(const std::vector<Tarea>& tareas, const std::vector<int>& orden);

// Informacion de cada tarea dentro de un orden, para mostrar el detalle.
struct FilaDetalle {
    int id;
    long long p, d, w, fin, retraso, penalizacion;
};

std::vector<FilaDetalle> detalle_orden(const std::vector<Tarea>& tareas,
                                       const std::vector<int>& orden);

#endif
