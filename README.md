Trabajo Práctico N2

# myshell

## Objetivos 
Con el desarrollo del siguiente Trabajo Práctico (TP), se busca:
- Utilizar mecanismos de creación de Procesos.
- Utilizar mecanismos de concurrencia e IPC.
- Diseñar un intérprete de línea de comandos al estilo Bourne shell.

## Introducción
Este trabajo práctico consta en la elaboración de un programa en lenguaje **C** sobre **GNU/Linux**. El trabajo se divide en soluciones incrementales.

## Command line prompt
_myshell_ debe contar con un prompt que contenga el camino al directorio actual. e.g.: 
```Bash
username@groupname:~$
```

## Internal commands
_myshell_ debe soportar los siguientes comandos internos:
- **cd \<directorio\>**: cambia el directorio actual a \<directorio\>. Si <directory> no está presente, reporta el directorio actual. Si el directorio no existe se debe imprimir un error apropiado. Además, este comando debe cambiar la variable de entorno PWD.
- **clr** : limpia la pantalla
- **echo \<comentario\>**: muestra <comentario> en la pantalla seguido por una línea nueva. (multiple espacios/tabs pueden ser reducidos a un espacio).
- **quit**:  cierra myshell

## Program invocation
Entradas del usuario que no sean comandos internos deben ser interpretados como la invocación de un programa. La misma tiene que ser realizada mediante **fork** y **exec**.

## Batch File
_myshell_ debe ser capaz de tomar sus comandos a ejecutar desde un archivo. Por ejemplo, la _shell_ puede ser invocada con un argumento **myshell batchfile**. El _batchfile_ contiene un conjunto de comandos de línea para que la shell ejecute. 

Cuando se alcance el fin de archivo (_EOF_), _myshell_ debe cerrarse.

Notar que si _myshell_ se ejecuta sin argumento (./myshell), se tiene que mostrar el command prompt y se debe esperar a comandos del usuario vía stdin.

## I/O redirection 
Se debe soportar redirección de entrada/salida en stdin y/o stdout. Por ejemplo:
```Bash
program arg1 ar2 < inputfile > outputfile
```
Ejecuta la el programa _program_ con los arguments _arg1_, _arg2_. stdin es reemplazado por inputfile y stdout por outputfile.

La redirección debe funcionar para el comando interno **echo**.

## Background execution
Un ampersand **&** al final de la línea de comando indica que la _shell_ debe retornar al prompt inmediatamente luego de lanzar al programa a ejecutarse en background.

Cuando se comienza un trabajo en background, se debe imprimir un mensaje indicando su Trabajo y su ID de proceso.
```Bash
[<job id>] <process id>
```
Ejemplo:
```Bash
$ echo 'hola' &
[1] 10506
hola
```
Cuando un trabajo es parado (stopped) por una señal, se debe imprimir el siguiente mensaje:
```Bash
[<job id>] <process id> suspended by signal <signal number>
```
Cuando un trabajo es continuado (resumed) por la señal **SIGCONT**, se debe imprimir el siguiente mensaje:
```Bash
[<job id>] <process id> resumed
```
Finalmente, gestionar correctamente los procesos lanzados en background para evitar la generación de procesos zombies.

## Signal Handling
Si se ingresa alguna de las combinaciones CTRL-C, CTRL-Z o CTRL-\, las señales resultantes (SIGINT, SIGTSTP, SIGQUIT respectivamente) deben ser enviadas al trabajo (job) en ejecución de primer plano en vez de a _myshell_. Si no hay un trabajo (job) en ejecución de primer plano, no debe suceder nada.

## Pipe
_myshell_ provee la funcionalidad de un pipe a través del operador ‘|’ (pipe). El mismo conecta la salida estándar del proceso lazando por el comando de la izquierda del pipe con la entrada estándar del proceso que se genera con el comando a la derecha del pipe.

Ejemplos:
```Bash
$ last <username> | wc -l
$ ps aux | grep firefox
$ grep bash /etc/passwd | cut -d “:” -f 1 | sort -r
```

### Responder:
¿Dónde se encuentran los pipes en el filesystem, qué atributos tienen?

## Criterios de Corrección
- Se debe compilar el código con los flags de compilación: -Wall -Pedantic 
- Dividir el código en módulos de manera juiciosa.
- Estilo de código.
- El código no debe contener errores, ni warnings.
- El código no debe contener errores de cppcheck.
- myshell no debe finalizar dejando procesos zombies.

## Qué se debe Entregar
- Informe del desarrollo del proyecto.
- Código (funcionando bajo las especificaciones dadas y bajo cualquier caso de test de parámetros de entrada).
- Makefile






