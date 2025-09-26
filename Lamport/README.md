# Sistema Distribuido Sincronizado con Algoritmo de Lamport (Windows)

## Descripción

Este ejemplo implementa la sincronización de procesos distribuidos (P1, P2, P3, P4) usando el **Algoritmo de Lamport** y **sockets TCP en Windows**.

Cada proceso es un ejecutable independiente que se comunica con el siguiente usando mensajes TCP y mantiene un reloj lógico de Lamport.

---

## Archivos incluidos

- `P1.cpp` — Proceso 1 (envía a P2)
- `P2.cpp` — Proceso 2 (envía a P3)
- `P3.cpp` — Proceso 3 (envía a P4)
- `P4.cpp` — Proceso 4 (último)

---

## Cómo compilar

Puedes compilar cada archivo con **Visual Studio** o con la línea de comandos `Developer Command Prompt for VS` de la siguiente manera:

```sh
cl P1.cpp /EHsc
cl P2.cpp /EHsc
cl P3.cpp /EHsc
cl P4.cpp /EHsc
```

Esto generará los ejecutables `P1.exe`, `P2.exe`, `P3.exe`, `P4.exe`.

---

## Cómo ejecutar

1. **Copia cada ejecutable en la computadora correspondiente**  
   Si es todo en una sola PC para pruebas, simplemente abre 4 terminales.

2. **Asegúrate que los puertos (5001, 5002, 5003, 5004) estén libres y no bloqueados por firewall.**

3. **Ejecuta primero P2, P3 y P4 (en ese orden), y por último ejecuta P1.**  
   Esto es porque cada proceso espera a que la conexión entrante esté lista.

   Por ejemplo, en cuatro ventanas diferentes:

   ```sh
   P2.exe
   P3.exe
   P4.exe
   P1.exe
   ```

   O si usas varias máquinas, asegúrate de cambiar la IP en cada archivo al IP correspondiente del siguiente proceso.

4. **Observa la salida en consola**:  
   Cada proceso mostrará cuándo recibe el mensaje, su reloj Lamport y cuándo ejecuta su acción.

---

## Configuración para varias computadoras

- Modifica la variable `next_ip` en cada archivo (`P1.cpp`, `P2.cpp`, `P3.cpp`) para poner la IP real del siguiente proceso.
- Si tienes dudas, pregunta a tu profesor qué IP debe escuchar cada proceso.

---

## ¿Cómo funciona?

- P1 inicia el ciclo, envía el mensaje a P2.
- P2 espera el mensaje, ejecuta su acción, incrementa el reloj y envía a P3.
- Así sucesivamente hasta P4.
- Cada mensaje contiene el reloj Lamport y el id de emisor.

---

## ¿Cómo lo adapto para ciclos infinitos?

- Puedes poner el bucle principal dentro de un `while(true)` y que P4 vuelva a enviar a P1 si quieres hacer rondas cíclicas.

---

**Cualquier duda, pregunta!**
