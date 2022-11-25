#include "syscall.h" //Muchas gracias al cientifico de la computacion Carlos Quesada Estrada por el programa de prueba
void hijo(int);
int id;
int main() {
    id = 3;
    Fork(hijo);
    Write("Hola !\n", 7, 1);
    Exit(0);
}

void hijo(int dummy) {
    Write("Hola ?\n", 7, 1);
}