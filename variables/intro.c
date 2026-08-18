/*
Crea un programa que guarde en variables:

    Tu nombre
    Tu edad
    Tu altura en metros
    Tu año de nacimiento

Después, muestra todos esos datos por pantalla.


*/
#include <stdio.h>

int main(void) {
    char name[] = "Agustin";
    int age = 21;
    float height = 1.75;
    int yearofbirth = 2005;

    printf("Nombre: %s\n", name);
    printf("Edad: %d\n", age);
    printf("Altura: %.2f metros\n", height);
    printf("Año de nacimiento: %d\n", yearofbirth);

    return 0;
}
