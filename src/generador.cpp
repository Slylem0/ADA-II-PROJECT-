// Genera una instancia aleatoria reproducible y la escribe en la salida estandar.
//
// Uso: ./generador n semilla [--holgura alta|media|baja]

#include <iostream>
#include <string>

#include "generacion.h"

int main(int argc, char* argv[]) {
    const char* uso = "Uso: ./generador n semilla [--holgura alta|media|baja]\n";
    if (argc != 3 && argc != 5) {
        std::cerr << uso;
        return 1;
    }

    int n;
    unsigned semilla;
    try {
        n = std::stoi(argv[1]);
        semilla = (unsigned)std::stoul(argv[2]);
    } catch (const std::exception&) {
        std::cerr << "Error: n y semilla deben ser enteros\n" << uso;
        return 1;
    }
    if (n < 0 || n > 1000) {
        std::cerr << "Error: n debe estar entre 0 y 1000\n";
        return 1;
    }

    Holgura holgura = Holgura::Media;
    if (argc == 5) {
        std::string opcion = argv[3], valor = argv[4];
        if (opcion != "--holgura") {
            std::cerr << uso;
            return 1;
        }
        if (valor == "alta") holgura = Holgura::Alta;
        else if (valor == "media") holgura = Holgura::Media;
        else if (valor == "baja") holgura = Holgura::Baja;
        else {
            std::cerr << "Error: holgura debe ser alta, media o baja\n";
            return 1;
        }
    }

    escribir_instancia(std::cout, generar_instancia(n, semilla, holgura));
    return 0;
}
