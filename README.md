# ADA II — Programación de tareas con penalización por retraso

Primer proyecto de Análisis y Diseño de Algoritmos II (Univalle).

## Problema

Hay `n` tareas que se ejecutan en una sola máquina, una a la vez y sin
interrupción. Cada tarea `j` tiene duración `p_j`, fecha límite `d_j` y
penalización `w_j` por cada unidad de tiempo de retraso. Se busca el orden que
minimiza

```
costo = Σ w_j · max(0, C_j − d_j)
```

donde `C_j` es el instante en que termina la tarea `j` (problema `1 || Σ w_j T_j`,
NP-difícil). El diseño completo está en
[`docs/especificacion-diseno.md`](docs/especificacion-diseno.md).

Algoritmos implementados:

| Algoritmo | Tipo | Complejidad |
|---|---|---|
| Fuerza bruta (`fb`) | exacto, solo para verificar (`n ≤ 10`) | `O(n · n!)` |
| Programación dinámica (`dp`) | exacto, sobre subconjuntos (`n ≤ 24`) | `O(2^n · n)` |
| Voraz EDD (`edd`) | heurístico: menor fecha límite primero | `O(n log n)` |
| Voraz WSPT (`wspt`) | heurístico: mayor `w/p` primero | `O(n log n)` |
| Voraz WMDD (`wmdd`) | heurístico: menor `max(p, d − t) / w` en cada paso | `O(n²)` |

## Compilar

Requiere `g++` con soporte de C++17 y `make`.

```bash
make          # compila tareas, generador, experimentos y pruebas
make test     # ejecuta todas las pruebas
make clean    # borra lo compilado
```

## Uso

### Resolver una instancia

```bash
./tareas ejemplos/ejemplo_basico.txt
./tareas ejemplos/voraz_no_optimo.txt --detalle
./tareas ejemplos/mediano_15.txt --algoritmo dp
./tareas < mi_instancia.txt
```

Opciones:

- `--algoritmo dp|fb|edd|wspt|wmdd|todos` (por defecto `todos`; con `todos` se
  omiten automáticamente los algoritmos exactos que no admiten ese `n`).
- `--detalle` muestra, para cada tarea, su fin, retraso y penalización.

Si se ejecuta un algoritmo exacto junto con los voraces, al final se muestra la
brecha de cada voraz frente al óptimo.

### Formato de entrada

```
# las líneas que empiezan con # se ignoran
n
p_1 d_1 w_1
...
p_n d_n w_n
```

Restricciones: `0 ≤ n ≤ 1000`, `1 ≤ p ≤ 10^6`, `0 ≤ d ≤ 10^9`, `0 ≤ w ≤ 10^6`.
Las tareas se numeran `1..n` en el orden en que aparecen.

### Generar instancias aleatorias

```bash
./generador 12 42 --holgura baja > instancia.txt
./generador 20 7 | ./tareas
```

`--holgura alta|media|baja` controla qué tan holgadas son las fechas límite
(por defecto `media`). La misma semilla produce siempre la misma instancia.

### Experimentos

```bash
./experimentos   # tarda alrededor de un minuto
```

Corre todos los algoritmos para `n = 4..24`, 5 semillas y las tres holguras, y
escribe `resultados/experimentos.csv` con las columnas
`n, semilla, holgura, algoritmo, costo, tiempo_ms, brecha`.

## Estructura

```
src/          código fuente (un módulo por algoritmo)
tests/        pruebas automáticas
ejemplos/     instancias de ejemplo
resultados/   resultados de los experimentos
docs/         especificación de diseño
```
