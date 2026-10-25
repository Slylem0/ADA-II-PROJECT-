#include "generacion.h"

#include <random>

std::vector<Tarea> generar_instancia(int n, unsigned semilla, Holgura holgura) {
    std::mt19937 gen(semilla);
    std::uniform_int_distribution<long long> valor(1, 10);

    std::vector<Tarea> tareas;
    long long suma = 0;
    for (int i = 1; i <= n; ++i) {
        long long p = valor(gen);
        long long w = valor(gen);
        tareas.push_back({i, p, 0, w});
        suma += p;
    }

    double factor = holgura == Holgura::Alta ? 1.0 : holgura == Holgura::Media ? 0.6 : 0.3;
    std::uniform_int_distribution<long long> fecha(0, (long long)(factor * suma));
    for (Tarea& t : tareas) t.d = fecha(gen);
    return tareas;
}

void escribir_instancia(std::ostream& out, const std::vector<Tarea>& tareas) {
    out << tareas.size() << "\n";
    for (const Tarea& t : tareas) out << t.p << " " << t.d << " " << t.w << "\n";
}
