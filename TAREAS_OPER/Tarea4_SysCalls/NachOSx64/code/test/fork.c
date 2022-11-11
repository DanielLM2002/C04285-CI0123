#include "syscall.h"
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