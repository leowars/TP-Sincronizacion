# Método de Cristian para Sincronización de Relojes

Este directorio contiene una implementación simple en Python del **método de Cristian** para sincronización de relojes en sistemas distribuidos. Consta de un servidor de tiempo y un cliente.

## Archivos

- `time_server.py`: Servidor de tiempo. Escucha conexiones y responde con su hora actual.
- `cristian_client.py`: Cliente que solicita la hora al servidor, calcula el RTT y ajusta su reloj estimado usando el método de Cristian.

## ¿Cómo funciona?

1. El cliente solicita la hora al servidor.
2. El servidor responde con su tiempo actual.
3. El cliente mide el tiempo de ida y vuelta (RTT) y ajusta su reloj considerando la mitad del RTT.

## Ejecución

1. Abre una terminal y ejecuta el servidor:
    ```bash
    python time_server.py
    ```
2. En otra terminal, ejecuta el cliente:
    ```bash
    python cristian_client.py
    ```

## Ejemplo de salida

```
RTT: 0.002134 segundos
Hora del servidor: Fri Sep 26 12:47:35 2025
Hora local antes: Fri Sep 26 12:47:35 2025
Desfase estimado: 0.001067 segundos
Hora local sincronizada estimada: Fri Sep 26 12:47:35 2025
```

## Notas

- Este ejemplo es solo educativo y no ajusta el reloj del sistema real.
- Ambos scripts usan `localhost` y el puerto `5000` por defecto.
- Puedes modificar el host y el puerto en los archivos si es necesario.

## Referencias

- Tanenbaum, A. S., & Van Steen, M. (2017). **Distributed Systems: Principles and Paradigms**.
- [Wikipedia: Cristian's algorithm](https://en.wikipedia.org/wiki/Cristian%27s_algorithm)
