Problemas con el echo y program invocation, en el que si escribia bien por ejemplo, la redireccion del input
pero habia un error en la escritura del output, me olvidaba de restaurar el input y el programa hacia cosas
rarisimas, como leer del archivo redireccionado e intentar ejecutar las lineas de ese archivo como si fueran
un program invocation.

Bugs conocidos: 
1_la redireccion de output provoca que el job id y el process id se impriman en el output seleccionado.

2_Como no hay forma de saber cuando el child process que esta en background va a retornar y printear cosas en consola, ese print se solapa con el prompt del parent process y queda un poco mal el print.

3_Al ejecutar clr en background y luego cerrar la consola, se imprimen tarde el jod id y el process id, junto con un aparente fallido en una ejecucion en segundo plano de un child process.