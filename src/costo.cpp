#include "costo.h"

#include <algorithm>

std::vector<FilaDetalle> detalle_orden(const std::vector<Tarea>& tareas,
                                       const std::vector<int>& orden) {
    std::vector<FilaDetalle> filas;
    long long tiempo = 0;
    for (int id : orden) {
        const Tarea& t = tareas[id - 1];
        tiempo += t.p;
        long long retraso = std::max(0LL, tiempo - t.d);
        filas.push_back({t.id, t.p, t.d, t.w, tiempo, retraso, t.w * retraso});
    }
    return filas;
}

long long calcular_costo(const std::vector<Tarea>& tareas, const std::vector<int>& orden) {
    long long costo = 0;
    for (const FilaDetalle& f : detalle_orden(tareas, orden)) costo += f.penalizacion;
    return costo;
}
