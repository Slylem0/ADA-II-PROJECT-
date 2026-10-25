# Especificación de diseño — Programación de tareas con penalización por retraso

## 1. Problema

Se tienen `n` tareas que se ejecutan en **una sola máquina**, una a la vez y sin
interrupción (no expropiativo), todas disponibles desde el instante 0.
Cada tarea `j` tiene:

| Símbolo | Significado | Restricción |
|---|---|---|
| `p_j` | duración (tiempo de procesamiento) | entero `> 0` |
| `d_j` | fecha límite (due date) | entero `>= 0` |
| `w_j` | penalización por unidad de retraso (peso) | entero `>= 0` |

Para un orden (permutación) `π`, la tarea `j` termina en `C_j` (suma de las
duraciones de las tareas que van antes que ella más la suya). Su retraso es
`T_j = max(0, C_j - d_j)`.

**Objetivo:** encontrar `π` que minimice el costo total

```
costo(π) = Σ_j  w_j · T_j
```

En notación de Graham es `1 || Σ w_j T_j` (*total weighted tardiness*).

### Pertenencia a NP y complejidad

- **Versión de decisión:** ¿existe un orden con `costo ≤ K`?
- **Está en NP:** el certificado es la permutación; se verifica en `O(n)`
  calculando los `C_j` y sumando `w_j · T_j`.
- **Es NP-difícil en sentido fuerte** (Lawler 1977; Lenstra, Rinnooy Kan y
  Brucker 1977), por lo que su versión de decisión es NP-completa y no se
  conoce algoritmo polinomial (ni siquiera pseudo-polinomial).
- **Espacio de búsqueda:** `n!` órdenes.

## 2. Algoritmos

### 2.1 Fuerza bruta (solo verificación)

Recorre las `n!` permutaciones con `std::next_permutation`.
Tiempo `O(n · n!)`, espacio `O(n)`. Límite práctico: `n ≤ 10`.
Sirve para comprobar que la programación dinámica da el óptimo.

### 2.2 Programación dinámica sobre subconjuntos (exacta)

**Idea clave:** si un conjunto de tareas `S` se ejecuta *primero* (en cualquier
orden), la última de ellas termina siempre en `P(S) = Σ_{j∈S} p_j`, sin importar
el orden interno. Así, el costo de la última tarea solo depende de `S`, no del
orden de las anteriores → subestructura óptima.

**Estado:** `f(S)` = costo mínimo de programar exactamente las tareas de `S` al
inicio del cronograma.

**Recurrencia:**

```
f(∅) = 0
f(S) = min_{j ∈ S} [ f(S \ {j}) + w_j · max(0, P(S) - d_j) ]
```

(`j` es la tarea que se ejecuta de última dentro de `S`.)

**Respuesta:** `f(U)` con `U` = todas las tareas. El orden se reconstruye
guardando en `ultima[S]` la `j` que alcanza el mínimo y retrocediendo desde `U`.

**Implementación:** los subconjuntos se representan como máscaras de bits
(`uint32_t`) y se recorren en orden creciente (todo subconjunto de `S` es menor
que `S`). `P(S)` se calcula dentro del mismo ciclo sobre los bits de `S`.

**Complejidad:** tiempo `O(2^n · n)`, espacio `O(2^n)`
(`int64` para `f` y `int8` para `ultima` ≈ 9 bytes por estado).
Límite: `n ≤ 24` (≈ 150 MB). Para `n` mayor el programa lo rechaza con un
mensaje claro.

Comparación con fuerza bruta: `2^n · n` frente a `n! · n`; para `n = 20`,
≈ 2·10⁷ frente a ≈ 4.9·10¹⁹ operaciones.

### 2.3 Algoritmos voraces (heurísticos)

Todos son `O(n log n)` o `O(n²)` y **no garantizan optimalidad**.

| Nombre | Criterio de selección | Complejidad |
|---|---|---|
| **EDD** (Earliest Due Date) | ordenar por `d_j` creciente | `O(n log n)` |
| **WSPT** (regla de Smith) | ordenar por `w_j / p_j` decreciente | `O(n log n)` |
| **WMDD** (Weighted Modified Due Date) — voraz principal | en cada paso, con tiempo actual `t`, elegir la tarea pendiente que minimice `max(p_j, d_j - t) / w_j` | `O(n²)` |

- **EDD** es óptimo cuando todos los pesos son iguales y como mucho una tarea
  queda tarde; ignora los pesos.
- **WSPT** es óptimo cuando todas las tareas van a quedar tarde
  (`d_j = 0` → `Σ w_j C_j`); ignora las fechas límite.
- **WMDD** combina ambas ideas y es dinámico (depende de `t`): si la tarea
  aún tiene holgura se comporta como EDD ponderado; si ya va tarde, como WSPT.
  Es el voraz que se defiende en el informe; EDD y WSPT sirven como
  referencia.
- Tareas con `w_j = 0`: no cuestan nada al retrasarse; en WMDD se tratan como
  prioridad mínima (se envían al final), y en WSPT su cociente es 0.
- **Contraejemplo de optimalidad** (para el informe): se buscará y documentará
  una instancia pequeña donde cada voraz no da el óptimo, encontrada con el
  comparador automático.

## 3. Programa

### 3.1 Formato de entrada (archivo o entrada estándar)

```
n
p_1 d_1 w_1
p_2 d_2 w_2
...
p_n d_n w_n
```

Las tareas se numeran `1..n` en el orden en que aparecen. Las líneas que
empiezan con `#` se ignoran (comentarios). Errores de formato o valores
inválidos → mensaje en `stderr` indicando la línea y código de salida `1`.

### 3.2 Ejecutable `tareas`

```
./tareas [archivo] [--algoritmo dp|fb|edd|wspt|wmdd|todos] [--detalle]
```

- Sin `archivo` lee de entrada estándar.
- Por defecto `--algoritmo todos` (fuerza bruta solo si `n ≤ 10`, DP solo si
  `n ≤ 24`).
- Salida por algoritmo: orden, costo total y tiempo de ejecución.
- `--detalle` añade la tabla por tarea: `j, p, d, w, C_j, T_j, w_j·T_j`.
- Si se corren varios, al final muestra la brecha de cada voraz frente al
  óptimo: `(costo_voraz - costo_opt) / costo_opt`.

### 3.3 Ejecutable `generador`

```
./generador n semilla [--holgura alta|media|baja]
```

Genera una instancia aleatoria reproducible en el formato de 3.1.
`p ∈ [1,10]`, `w ∈ [1,10]`, `d` uniforme en `[0, factor · Σp]` según la
holgura (baja = fechas apretadas → más tareas tarde).

### 3.4 Ejecutable `experimentos`

Para `n = 4..24` y varias semillas por `n`, genera instancias, ejecuta DP y
los tres voraces (y fuerza bruta para `n ≤ 10`) y escribe un CSV en
`resultados/experimentos.csv` con columnas:

```
n, semilla, holgura, algoritmo, costo, tiempo_ms, brecha
```

Estos datos alimentan las secciones de pruebas y análisis del informe
(tiempo vs. `n`, calidad del voraz vs. `n` y vs. holgura).

## 4. Estructura del repositorio

```
src/
  tarea.h           struct Tarea {id, p, d, w}; tipos comunes
  entrada.h/.cpp    lectura y validación de instancias
  costo.h/.cpp      costo de un orden + tabla de detalle
  fuerza_bruta.h/.cpp
  dp.h/.cpp
  voraz.h/.cpp      EDD, WSPT, WMDD
  main.cpp          ejecutable tareas
  generador.cpp
  experimentos.cpp
tests/
  pruebas.cpp       pruebas automáticas (sin librerías externas)
ejemplos/           instancias diseñadas por el equipo (*.txt)
resultados/         CSV generados por experimentos
docs/               esta especificación
Makefile
README.md           cómo compilar y ejecutar
```

Cada algoritmo expone la misma interfaz:

```cpp
struct Resultado { std::vector<int> orden; long long costo; };
Resultado resolver_xxx(const std::vector<Tarea>& tareas);
```

Compilación: `g++ -std=c++17 -O2 -Wall -Wextra`, mediante `make`.
Sin dependencias externas.

## 5. Pruebas (`make test`)

1. **Costo:** casos calculados a mano (incluye el ejemplo A/B/C del informe).
2. **DP = fuerza bruta:** cientos de instancias aleatorias con `n ≤ 8` y
   distintas holguras; el costo de la DP debe coincidir siempre.
3. **Validez:** todo orden devuelto es una permutación de `1..n` y su costo
   recalculado coincide con el reportado.
4. **Cota:** costo de cada voraz `≥` costo de la DP.
5. **Casos borde:** `n = 0`, `n = 1`, todas a tiempo (costo 0), todas tarde,
   pesos 0, empates.
6. **Entrada:** archivos mal formados producen error con código de salida 1.

## 6. Fuera de alcance

- Interfaz gráfica o menú interactivo.
- Gráficas (se generan aparte a partir del CSV).
- El informe en PDF (es un entregable aparte).
