// Programa principal: lee una instancia y la resuelve con los algoritmos pedidos.
//
// Uso: ./tareas [archivo] [--algoritmo dp|fb|edd|wspt|wmdd|todos] [--detalle]
// Sin archivo lee de la entrada estandar.

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "costo.h"
#include "dp.h"
#include "entrada.h"
#include "fuerza_bruta.h"
#include "voraz.h"

namespace {

struct Algoritmo {
    std::string clave;
    std::string nombre;
    Resultado (*resolver)(const std::vector<Tarea>&);
    int max_n;     // -1 si no tiene limite
    bool exacto;   // true si garantiza el optimo
};

const std::vector<Algoritmo> ALGORITMOS = {
    {"fb", "Fuerza bruta", resolver_fuerza_bruta, MAX_N_FB, true},
    {"dp", "Programacion dinamica", resolver_dp, MAX_N_DP, true},
    {"edd", "Voraz EDD", resolver_edd, -1, false},
    {"wspt", "Voraz WSPT", resolver_wspt, -1, false},
    {"wmdd", "Voraz WMDD", resolver_wmdd, -1, false},
};

struct Opciones {
    std::string archivo;  // vacio = entrada estandar
    std::string algoritmo = "todos";
    bool detalle = false;
};

void uso() {
    std::cerr << "Uso: ./tareas [archivo] [--algoritmo dp|fb|edd|wspt|wmdd|todos] [--detalle]\n";
}

Opciones leer_opciones(int argc, char* argv[]) {
    Opciones op;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--detalle") {
            op.detalle = true;
        } else if (arg == "--algoritmo") {
            if (i + 1 >= argc) throw std::runtime_error("falta el valor de --algoritmo");
            op.algoritmo = argv[++i];
        } else if (arg.rfind("--", 0) == 0) {
            throw std::runtime_error("opción desconocida '" + arg + "'");
        } else if (op.archivo.empty()) {
            op.archivo = arg;
        } else {
            throw std::runtime_error("solo se admite un archivo de entrada");
        }
    }
    if (op.algoritmo != "todos") {
        bool existe = false;
        for (const Algoritmo& a : ALGORITMOS) existe = existe || a.clave == op.algoritmo;
        if (!existe) throw std::runtime_error("algoritmo desconocido '" + op.algoritmo + "'");
    }
    return op;
}

void imprimir_detalle(const std::vector<Tarea>& tareas, const std::vector<int>& orden) {
    std::printf("%6s %8s %10s %8s %10s %9s %13s\n", "Tarea", "p", "d", "w", "Fin", "Retraso",
                "Penalizacion");
    for (const FilaDetalle& f : detalle_orden(tareas, orden)) {
        std::printf("%6d %8lld %10lld %8lld %10lld %9lld %13lld\n", f.id, f.p, f.d, f.w, f.fin,
                    f.retraso, f.penalizacion);
    }
}

std::string brecha(long long costo, long long optimo) {
    if (optimo == 0) return costo == 0 ? "0.00 %" : "inf";
    char texto[32];
    std::snprintf(texto, sizeof texto, "%.2f %%", 100.0 * (costo - optimo) / optimo);
    return texto;
}

}  // namespace

int main(int argc, char* argv[]) {
    Opciones op;
    std::vector<Tarea> tareas;
    try {
        op = leer_opciones(argc, argv);
        if (op.archivo.empty()) {
            tareas = leer_instancia(std::cin);
        } else {
            std::ifstream archivo(op.archivo);
            if (!archivo) throw std::runtime_error("no se pudo abrir '" + op.archivo + "'");
            tareas = leer_instancia(archivo);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        uso();
        return 1;
    }

    int n = (int)tareas.size();
    std::cout << "Instancia con " << n << " tareas\n\n";

    std::optional<long long> optimo;
    std::vector<std::pair<std::string, long long>> voraces;

    for (const Algoritmo& a : ALGORITMOS) {
        if (op.algoritmo != "todos" && op.algoritmo != a.clave) continue;
        if (op.algoritmo == "todos" && a.max_n != -1 && n > a.max_n) {
            std::cout << "(" << a.nombre << " omitida: admite como maximo " << a.max_n
                      << " tareas)\n\n";
            continue;
        }

        Resultado r;
        auto inicio = std::chrono::steady_clock::now();
        try {
            r = a.resolver(tareas);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
        auto fin = std::chrono::steady_clock::now();
        double ms = std::chrono::duration<double, std::milli>(fin - inicio).count();

        std::cout << "== " << a.nombre << " ==\nOrden:";
        for (int id : r.orden) std::cout << " " << id;
        std::cout << "\nCosto total: " << r.costo << "\n";
        std::printf("Tiempo: %.3f ms\n", ms);
        std::fflush(stdout);
        if (op.detalle) imprimir_detalle(tareas, r.orden);
        std::cout << "\n";

        if (a.exacto) optimo = r.costo;
        else voraces.push_back({a.nombre, r.costo});
    }

    if (optimo && !voraces.empty()) {
        std::cout << "== Brecha frente al optimo (" << *optimo << ") ==\n";
        for (const auto& [nombre, costo] : voraces) {
            std::cout << nombre << ": " << brecha(costo, *optimo) << "\n";
        }
    }
    return 0;
}
