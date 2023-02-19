# ejercicio1-arquitectura

## Comandos de compilación

- Compilar el servidor: `gcc -o servidor.out servidor.c -lrt && ./servidor.out`
- Compilar el cliente: `gcc -o cliente.out cliente.c -lrt && ./cliente.out`
  El -lrt se usa para que el programa pueda usar las funciones de tiempo real.
- Para mirar la cola de mensajes: `ls /dev/mqueue/`
- Para leer el contenido de la cola de mensajes: `cat /dev/mqueue/<nombre de la cola>`

- Para eliminar la cola de mensajes: `rm /dev/mqueue/<nombre de la cola>`
  > `rm /dev/mqueue/mq_server`

  > `rm /dev/mqueue/mq_client_0`

Se ha cambiado el nombre del archivo servidor.h a request.h por ser más descriptivo y seguir con el video.
Se ha añadido un response.h para no mezclar archivos y que todo quede mas limpio.

Tal vez deberíamos organizar los archivos en carpetas, quedará todo más ordenado.

P.D: perdón por usar comentarios bonitos, si no los puedes ver, :sad:

### TODO's

- TODO: estandarizar los comentarios entre los archivos servidor.c y cliente.c
- TODO: había intentado hacer char i=0; while(i!=-1) y así poder devolver la response  "ACK%d" pero es una movida y habría que importar otras librerías (no sé si se puede), asique lo dejé con while(1).
