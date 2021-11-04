# Trabajo Práctico 3

Materia: Taller de Programación I (75.42)

Alumno: Francisco Viñas

Padrón: 103207

Link al repositorio: https://github.com/franvinas/taller1-tp3


## Introducción

El trabajo práctico consiste en implementar un sistema cliente-servidor, en el que el servidor almacena colas que serán accesibles por todos los clientes. El servidor puede ejecutarse a través del comando:

~~~
./server <service>
~~~

Donde `service` es el puerto donde el servidor estará esperando nuevos clientes. El servidor detiene su ejecución una vez que lee de entrada estandar el caracter 'q'. El cliente, por otro lado, se ejecuta a través del comando:

~~~
./client <host> <service>
~~~

Una vez ejecutado el cliente, este se conecta al servidor dado los parametros `host` y `service`. El cliente lee de entrada estandar los comandos que el usuario ingresa, estos comandos son: `define`, `push`, `pop` y `exit`. El comando `exit` tiene la particularidad que no le envía ningun mensaje al servidor, simplemente provoca el cierre del cliente. A traves de los otros 3 comandos, el cliente se comunica con el servidor. Para estos tres casos, el cliente lee el comando y sus parametros, lo parsea y le envía un mensaje al servidor siguiendo un protocolo de comunicación.

El comando `define` tiene el siguiente formato:

~~~
define <nombre-de-la-cola>
~~~

Este comando crea una nueva cola vacía con el nombre dado por parametro. Si previamente ya existía una cola con el mismo nombre, no se hace nada.

El comando `push` tiene el siguiente formato:

~~~
push <nombre-de-la-cola> <mensaje-como-string>
~~~

Este comando encola un mensaje en la cola que corresponde segun los parametros dados. Si la cola no existía previamente, se crea una vacía y se encola el mensaje.

El comando `pop` tiene el siguiente formato:

~~~
pop <nombre-de-la-cola>
~~~

Este comando sirve para consumir un mensaje de una cola. En primer lugar se desencola un mensaje de la cola correspondiente y luego se imprime en el cliente el mensaje por salida estandar. Si la cola no existe o existe pero está vacia, el `pop` se quedará bloqueado hasta que haya un mensaje.

**Nota:** Las colas son compartidas entre todos los clientes, por lo tanto, si un cliente consume un mensaje de una cola, los demas clientes ya no podrán consumir ese mensaje.

## Descripcion de clases

La documentación completa de las clases, con las explicaciones de cada método se encuentra en los archivos `*.h` a modo de comentarios. A continuación se presenta una pequeña descripción de cada clase.

#### Socket

La clase `Socket` es común al cliente y al servidor. Provee una interfaz lo más reducida posible para usar sockets.

### Cliente

#### Client

La clase `Client` encapsula la lógica para comunicarse con el usuario. Se debe indicar la dirección del servidor y el puerto al cual conectarse. Una vez que se ejecuta el cliente (`run()`), comienza un ciclo donde se lee por entrada estandar los comandos del usuario y estos son delegados a la clase `Protocol` para que sean envíados al servidor. Cuando el usuario ingresa el comando `exit` finaliza la ejecución del cliente.

#### Protocol

La clase `Protocol` del cliente permite conectarse al servidor y comunicarse con este. El metodo `send` recibe por parametro un string con el comando que el usuario ingreso por teclado. La clase se ocupa de parsear el comando y enviarlo al servidor respetando el protocolo de comunicación. El método `recv` sirve para recibir un mensaje del cliente. Este método debe ser llamado despues de un `pop` para poder recibir el mensaje consumido.

### Servidor

#### Server

Esta clase permite crear un socket que escuche clientes que se conectan al puerto asignado. Con método `run` se entra en un ciclo donde se lee de entrada estandar, una vez que el usuario ingresa el caracter 'q' se interrumpen todas las conexiones con los clientes y se cierra el servidor. Dentro de este método se crea y ejecuta el hilo aceptador.

#### Thread

**Autoría:** El autor de esta clase es la cátedra Veiga de la materia Taller de Programación I (7542/9508) de la Facultad de Ingeniería de la UBA.

Esta es una clase abstracta. Toda clase que herede de `Thread` debe implementar el método protegido `run()`. La interfaz de esta clase consta con las funciones `start()` y `join()`. La primera de estas funciones lanza un hilo que corre la función `run()`. Por otro lado, la segunda de estas funciones, espera a que termine la ejecución del hilo.

#### AcceptorThread (hereda de Thread)

La clase `AcceptorThread` (o hilo aceptador) recibe el socket que aceptará clientes. La ejecución de este hilo finaliza una vez que este socket cierra. Durante su ejecución, por cada socket aceptado, se llevan adelante dos operaciones. En primer lugar, se lanza un hilo donde se dá a cabo la comunicación con el cliente. Y en segundo lugar, se itera sobre la estructura que almacena los hilos lanzados y se librean aquellos que ya finalizaron su ejecución. 

Debe existir una única instancia de `AcceptorThread`. En esta, vive la estructura donde se almacenan las colas compartidas (`QueuesMap`).

#### ServerThread (hereda de Thread)

Cada instancia de `ServerThread` tiene una referencia a la estructura donde se almacenan las colas (`QueuesMap`). Es una referencia porque todas las instancias deben poder acceder a esta estructura. 

La ejecución de estos hilos consiste en un ciclo donde se reciben comandos del cliente, se los parsea y luego se llama al metodo de `QueuesMap` que corresponda.

#### QueuesMap

Esta clase es una diccionario donde se almacenan las colas compartidas. Las claves son los nombres de las colas y los valores son las colas (instancias de `Queue`). Existen unicamente tres métodos, uno por cada comando válido: `define`, `push` y `pop`. Cada uno de estos busca la cola que corresponde según el nombre que se recibió por parametro y delega la operación a la cola.

#### Queue

Este clase implementa una cola. Tiene unicamente tres métodos: `define`, `pop` y `push`.

#### Protocol

La clase `Protocol` del servidor permite conectarse a un cliente y comunicarse con este. El metodo `recv` recibe el comando que el cliente quiere ejecutar. Dentro de este método se interpretan los bytes recibidos para obtener el comando a ejecutar, el nombre de la cola y el mensaje a encolar (si el comando es un `push`). El método `send` sirve para enviar un mensaje del cliente. Este método debe ser llamado despues de un `pop` para poder enviar el mensaje consumido.

## Diagramas 

## Consideraciones

- Si el cliente quiere realizar un `push` sobre una cola que no existe (no fue definida con `define`) se crea la cola y luego se realiza el `push`.
