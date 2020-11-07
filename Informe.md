Problemas con el echo y program invocation, en el que si escribia bien por ejemplo, la redireccion del input
pero habia un error en la escritura del output, me olvidaba de restaurar el input y el programa hacia cosas
rarisimas, como leer del archivo redireccionado e intentar ejecutar las lineas de ese archivo como si fueran
un program invocation.

Al mandar una SIGSTOP a un child process todos los comandos subsecuentes se ejecutaban en background hasta que mandaba una señal de SIGCONT. Este bug me tomo VARIAS horas y se debia a una flag mal colocada en un waitpid.

Bugs conocidos: 
1_la redireccion de output provoca que el job id y el process id se impriman en el output seleccionado.

2_Como no hay forma de saber cuando el child process que esta en background va a retornar y printear cosas en consola, ese print se solapa con el prompt del parent process y queda un poco mal el print.

3_Al ejecutar clr en background y luego cerrar la consola, se imprimen tarde el jod id y el process id, junto con un aparente fallido en una ejecucion en segundo plano de un child process.

4_Cuando el child process no encuentra el programa a ejecutar, no pareciera enviar una señal de terminacion, por lo que no se imprime un mensaje notificando que el proceso haya terminado.

5_Si se deja un sleep justo despues de que el parent process vuelva de dejar en background a un child, todos los comandos subsecuentes se ejecutan en brackground (???????????????????????????). Obviamente no voy a poner ese sleep.

6_Muchisimos bugs asociados al signal handling, sobretodo si se quiere mandar un sleep 60 al bakground y luego se manda un ctrl+z se bugea todo el programa. Ademas, el ctrl+c a veces provoca la impresion de varios prompts.