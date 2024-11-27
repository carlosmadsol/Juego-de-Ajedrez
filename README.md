# Juego-de-Ajedrez
Proyecto final de programacion bajo plataformas abiertas 
La finalidad del proyecto es hacer un juego de ajedrez jugable por dos jugadores en la misma maquina. Tiene una interfaz grafica que contiene el tablero para jugar y funcióna con el click del mouse.
Para su elaboracion se deben instalar ciertas dependencias: 

sudo apt update sudo apt install libgtk-4-dev 
// Esto es para instalar el gtk4

sudo apt install build-essential 
// Incluye gcc y make

sudo apt install pkg-config 
// Lo utilizamos para encontrar las rutas de los archivos necesarios durante la compilación.

Ademas recordar que ocupamos la biblioteca de kgchess para que corra el programa esta biblioteca fue modificada para la optimización del funciónamiento de la logica del ajedrez en la practica, ya que no estaba identificando si los movientos realizados eran validos o no

Para compilar es muy sencillo. Se le adjunto un makefile para hacer esto más sencillo
Para compilarlo solo se corre:
make
./ajedrez
make clean para limpiar
