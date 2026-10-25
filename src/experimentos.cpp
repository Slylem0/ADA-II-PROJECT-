// Ejecuta todos los algoritmos sobre instancias aleatorias de tamano creciente y
// guarda los resultados en un CSV para el analisis del informe.
//
// Uso: ./experimentos [salida.csv]   (por defecto resultados/experimentos.csv)

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dp.h"
#include "fuerza_bruta.h"
#include "generacion.h"
#include "voraz.h"

namespace {

const int N_MIN = 4;
const int N_MAX = MAX_N_DP;
const unsigned SEMILLAS = 5;

struct Medicion {
    long long costo;
    double ms;
};

Medicion medir(Resultado (*resolver)(const std::vector<Tarea>&), const std::vector<Tarea>& t) {
    auto inicio = std::chrono::steady_clock::now();
    Resultado r = resolver(t);
    auto fin = std::chrono::steady_clock::now();
    return {r.costo, std::chrono::duration<double, std::milli>(fin - inicio).count()};
}

// Brecha porcentual del voraz frente al optimo (vacia para los algoritmos exactos).
std::string brecha(long long costo, long long optimo) {
    if (optimo == 0) return costo == 0 ? "0.00" : "inf";
    char texto[32];
    std::snprintf(texto, sizeof texto, "%.2f", 100.0 * (costo - optimo) / optimo);
    return texto;
}

}  // namespace

int main(int argc, char* argv[]) {
    std::string ruta = argc > 1 ? argv[1] : "resultados/experimentos.csv";
    std::ofstream csv(ruta);
    if (!csv) {
        std::cerr << "Error: no se pudo crear '" << ruta << "'\n";
        return 1;
    }
    csv << "n,semilla,holgura,algoritmo,costo,tiempo_ms,brecha\n";

    const std::vector<std::pair<Holgura, std::string>> holguras = {
        {Holgura::Alta, "alta"}, {Holgura::Media, "media"}, {Holgura::Baja, "baja"}};
    const std::vector<std::pair<std::string, Resultado (*)(const std::vector<Tarea>&)>>
        voraces = {{"edd", resolver_edd}, {"wspt", resolver_wspt}, {"wmdd", resolver_wmdd}};

    for (int n = N_MIN; n <= N_MAX; ++n) {
        std::cerr << "n = " << n << "\n";
        for (unsigned semilla = 1; semilla <= SEMILLAS; ++semilla) {
            for (const auto& [holgura, nombre_holgura] : holguras) {
                auto tareas = generar_instancia(n, semilla, holgura);
                auto fila = [&](const std::string& algoritmo, Medicion m, const std::string& b) {
                    char ms[32];
                    std::snprintf(ms, sizeof ms, "%.4f", m.ms);
                    csv << n << "," << semilla << "," << nombre_holgura << "," << algoritmo
                        << "," << m.costo << "," << ms << "," << b << "\n";
                };

                Medicion dp = medir(resolver_dp, tareas);
                fila("dp", dp, "");
                if (n <= MAX_N_FB) fila("fb", medir(resolver_fuerza_bruta, tareas), "");
                for (const auto& [nombre, resolver] : voraces) {
                    Medicion m = medir(resolver, tareas);
                    fila(nombre, m, brecha(m.costo, dp.costo));
                }
            }
        }
    }
    std::cerr << "Resultados en " << ruta << "\n";
    return 0;
}
