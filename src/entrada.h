#ifndef ENTRADA_H
#define ENTRADA_H

#include <istream>
#include <vector>

#include "tarea.h"

// Lee una instancia con el formato:
//   n
//   p_1 d_1 w_1
//   ...
//   p_n d_n w_n
// Ignora lineas vacias y lineas que empiezan con '#'.
// Lanza std::runtime_error("linea K: <motivo>") si la entrada es invalida.
std::vector<Tarea> leer_instancia(std::istream& in);

#endif
