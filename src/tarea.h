#ifndef TAREA_H
#define TAREA_H

#include <vector>

// Una tarea j: duracion p, fecha limite d y penalizacion w por unidad de retraso.
// Los ids van de 1 a n segun el orden de aparicion en la entrada.
struct Tarea {
    int id;
    long long p, d, w;
};

// Resultado comun de todos los algoritmos: orden de ejecucion (ids) y su costo.
struct Resultado {
    std::vector<int> orden;
    long long costo;
};

#endif
