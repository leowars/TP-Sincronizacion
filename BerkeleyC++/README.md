# BerkeleyCplus

Implementación en C++ del Algoritmo de Berkeley para sincronización de relojes entre tres computadoras (un maestro y dos esclavos) utilizando sockets y threads.

## Archivos

- `master.cpp`: Proceso maestro, inicia la sincronización y coordina los ajustes de reloj.
- `slave.cpp`: Proceso esclavo, responde al maestro y ajusta su reloj en el puerto 10001.
- `slave2.cpp`: Proceso esclavo, responde al maestro y ajusta su reloj en el puerto 10002.

## Compilación

Se recomienda compilar usando g++ (requiere soporte de C++11 o superior):

```bash
g++ master.cpp -o master -pthread
g++ slave.cpp -o slave -pthread
g++ slave2.cpp -o slave2 -pthread
```

## Ejecución

1. Abre 3 terminales.
2. En la primera terminal, ejecuta el primer esclavo (puerto 10001):
   ```bash
   ./slave
   ```
3. En la segunda terminal, ejecuta el segundo esclavo (puerto 10002):
   ```bash
   ./slave2
   ```
4. En la tercera terminal, ejecuta el maestro:
   ```bash
   ./master
   ```

Cada proceso simula un reloj local y realiza la sincronización periódicamente.

---

**Requisitos:**  
- Sistema operativo compatible con sockets POSIX (Linux, Mac).  
- Tener libres los puertos 10001 y 10002 en localhost.

---
