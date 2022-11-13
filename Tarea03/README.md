# C04285-CI0123 Tarea 3 SERVER/CLIENT
El codigo de este si se encuentra en la carpeta correspondiente a la tarea 3 debido a que no se pedía releases para esta tarea.
## Compilación
### IMPORTANTE SEGUIR LOS PASOS
-------
## Primer paso a realizar
Para iniciar debemos de ejecutar el comando en la terminal de linux 
```
hostname -I
```
Esto nos dará la ip de la máquina, debemos de copiarla y pegarla en el primer parámetro de la linea 39 del main. Si no hacemos esto no podremos conectarnos al servidor.

----

## Pasos para el servidor
El main cuenta con un DEFINE en la linea 6 del mismo, para poder compilar y ejecutar correctamente debemos ir al define y cambiar el valor definido por un:

```
#define SERVER
```
Una vez hecho esto podemos compilar y ejecutar con el comando:
```
make run
```
Con esto ya tendremos un servidor ejecutandose, ***ES IMPORTANTE NO CERRAR LA TERMINAL EN LA QUE SE ESTA EJECUTANDO.***

---

## Pasos para el cliente

Despues debemos abrir otra terminal, ir al código y cambiar el define por:
```
#define CLIENT
```
Guardamos el archivo y ejecutamos el comando:
```
make clean && make && ./bin/Tarea03 saludos
```
***ES MUY IMPORTANTE HACER MAKE CLEAN***

Para poder compilar y ejecutar, es importante realizarlo de esta manera ya que debemos pasarle un parámetro al cliente para que este pueda enviarlo al servidor. Veremos la respuesta del servidor en la terminal donde se ejecuto el cliente.
