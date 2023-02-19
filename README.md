# ejercicio1-arquitectura

## Comandos de compilación

- Compilar el servidor: `gcc -o servidor.out servidor.c -lrt && ./servidor.out`
- Compilar el cliente: `gcc -o cliente.out cliente.c -lrt && ./cliente.out`
  El -lrt se usa para que el programa pueda usar las funciones de tiempo real.
- Para mirar la cola de mensajes: `ls /dev/mqueue/`
- Para leer el contenido de la cola de mensajes: `cat /dev/mqueue/<nombre de la cola>`

Se ha cambiado el nombre del archivo servidor.h a request.h por ser más descriptivo y seguir con el video.

P.D: perdón por usar comentarios bonitos, si no los puedes ver, :sad: