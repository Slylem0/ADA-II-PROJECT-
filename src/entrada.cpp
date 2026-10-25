#include "entrada.h"

#include <sstream>
#include <stdexcept>
#include <string>

namespace {

const long long MAX_N = 1000;
const long long MAX_P = 1000000;
const long long MAX_D = 1000000000;
const long long MAX_W = 1000000;

// Lee la siguiente linea util (no vacia ni comentario) y la deja en `linea`.
// Devuelve false si se acabo la entrada.
bool siguiente_linea(std::istream& in, std::string& linea, int& numero) {
    while (std::getline(in, linea)) {
        ++numero;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        // Marca BOM de UTF-8 que algunos editores ponen al inicio del archivo
        if (numero == 1 && linea.rfind("\xEF\xBB\xBF", 0) == 0) linea.erase(0, 3);
        size_t inicio = linea.find_first_not_of(" \t");
        if (inicio == std::string::npos || linea[inicio] == '#') continue;
        return true;
    }
    return false;
}

[[noreturn]] void error(int numero, const std::string& motivo) {
    throw std::runtime_error("línea " + std::to_string(numero) + ": " + motivo);
}

// Extrae exactamente `cantidad` enteros de la linea; falla si sobra o falta algo.
std::vector<long long> leer_enteros(const std::string& linea, int numero, size_t cantidad) {
    std::istringstream ss(linea);
    std::vector<long long> valores;
    std::string token;
    while (ss >> token) {
        size_t usados = 0;
        long long valor;
        try {
            valor = std::stoll(token, &usados);
        } catch (const std::exception&) {
            error(numero, "valor no numérico '" + token + "'");
        }
        if (usados != token.size()) error(numero, "valor no numérico '" + token + "'");
        valores.push_back(valor);
    }
    if (valores.size() != cantidad) {
        error(numero, "se esperaban " + std::to_string(cantidad) + " valores y hay " +
                          std::to_string(valores.size()));
    }
    return valores;
}

void validar_rango(long long valor, long long minimo, long long maximo, const char* nombre,
                   int numero) {
    if (valor < minimo || valor > maximo) {
        error(numero, std::string(nombre) + " = " + std::to_string(valor) +
                          " fuera de rango [" + std::to_string(minimo) + ", " +
                          std::to_string(maximo) + "]");
    }
}

}  // namespace

std::vector<Tarea> leer_instancia(std::istream& in) {
    std::string linea;
    int numero = 0;
    if (!siguiente_linea(in, linea, numero)) error(numero, "falta la cantidad de tareas n");

    long long n = leer_enteros(linea, numero, 1)[0];
    validar_rango(n, 0, MAX_N, "n", numero);

    std::vector<Tarea> tareas;
    for (int i = 1; i <= n; ++i) {
        if (!siguiente_linea(in, linea, numero)) {
            error(numero, "se esperaban " + std::to_string(n) + " tareas y solo hay " +
                              std::to_string(i - 1));
        }
        auto v = leer_enteros(linea, numero, 3);
        validar_rango(v[0], 1, MAX_P, "p", numero);
        validar_rango(v[1], 0, MAX_D, "d", numero);
        validar_rango(v[2], 0, MAX_W, "w", numero);
        tareas.push_back({i, v[0], v[1], v[2]});
    }

    if (siguiente_linea(in, linea, numero)) error(numero, "hay más tareas de las indicadas por n");
    return tareas;
}
