#myshell

##Metodos de uso
 Para compilar el programa, es suficiente escribir 
     
     make
Para ejecutar el programa, es suficiente escribir 

    ./myshell
 **Se recomienda tener la consola en pantalla completa para evitar bugs.**
 
  -  _myshell_  acepta los comandos internos **echo**, **cd**, **clr**, y **quit**.
 
 
  - _myshell_  acepta **invocaciones a programas**.
  
  
  - _myshell_  ejecutar tanto comandos internos como invocaciones en background, usando el caracter especial **&**. Se imprime un mensaje con el _job id_  y el _process id__  .
  
  
  - _myshell_  redirecciona las señales **ctrl-z** y **ctrl-c** al primer plano, y notifica cuando es recibida.
  
  
  - _myshell_  acepta **input/output redireccion** que funciona tanto con invocaciones a programas, como con comandos internos. 
  
  
   - _myshell_  acepta **pipes** mediante el operador **|** que permiten que los comandos que ingresa el usuario se comuniquen.
   
##Problemas encontrados

 - Problemas con el echo y program invocation, en el que si escribia bien por ejemplo, la redireccion del input
pero habia un error en la escritura del output, me olvidaba de restaurar el input y el programa hacia cosas
rarisimas, como leer del archivo redireccionado e intentar ejecutar las lineas de ese archivo como si fueran
un program invocation.


 - Al mandar una SIGSTOP a un child process todos los comandos subsecuentes se ejecutaban en background hasta que mandaba una señal de SIGCONT. Este bug me tomo VARIAS horas y se debia a una flag mal colocada en un waitpid.
 
 
  - Problemas con sigsetjmp que hasta ahora no entiendo por que se debia, pero se solucionaba cambiandolo de lugar.
  
##Bugs conocidos
 
- La redireccion de output provoca que el job id y el process id se impriman en el output seleccionado.


- Como no hay forma de saber cuando el child process que esta en background va a retornar y printear cosas en consola, ese print se solapa con el prompt del parent process y queda un poco mal el print.


 - Al ejecutar clr en background y luego cerrar la consola, se imprimen tarde el jod id y el process id, junto con un aparente fallido en una ejecucion en segundo plano de un child process.


 - Cuando el child process no encuentra el programa a ejecutar, no pareciera enviar una señal de terminacion, por lo que no se imprime un mensaje notificando que el proceso haya terminado.


 - El ctrl-z y ctrl-c mandan su signal tambien a algun proceso que este en background.
 
 
 - Usar pipes junto al I/O redirection no es compatible, por lo que puede causar resultados inesperados. Por ejemplo, si se redirecciona la salida del comando ps y se lo conecta a traves de un pipe a wc -l, este ultimo no va a encontrar el input esperado.
 
 
  - Antes de entregar me di cuenta que cuando uso pipes, los procesos no se limpian correctamente y quedan zombies.

##Ejempos de uso

Input:

    ping -c 5 google.com | grep rtt
    
Output: 

    rtt min/avg/max/mdev = 34.101/35.444/38.743/1.681 ms
    
Notar que el output en este caso se solapa con el prompt, pero no hay forma de evitarlo.
Este comando puede ser enviado a background con **&**, pero debe escribirse **una sola vez** ya que si se escribe mas de una vez,   _myshell_  toma ese segundo **&** como un argumento del programa a ejecutar.

   