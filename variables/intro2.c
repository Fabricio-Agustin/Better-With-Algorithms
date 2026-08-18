/*
Crea un programa que guarde en variables:

    El nombre de un producto.
    El precio del producto.
    La cantidad que compraste.
    El precio total.
*/

#include <stdio.h>

int main(void) {
    char name[] = "Pepsi";
    float price = 20.100;
    int amount = 10000;
    float total = price * amount;

    printf("Producto: %s\n", name);
    printf("Precio: %.2f\n", price);
    printf("Cantidad: %d\n", amount);
    printf("Total: %.2f\n", total);

    return 0;
}
