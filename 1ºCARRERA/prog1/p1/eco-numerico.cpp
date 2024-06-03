/*******************************************************************************
 * Curso de Programación 1. Tema 1
 * Autores: Miguel Ángel Latre
 * Modificado por: ¡PON AQUÍ TU NOMBRE!
 * Última revisión: 18 de septiembre de 2021
 * Resumen: Tarea 3 y 4
 ******************************************************************************/
#include <iostream>
using namespace std;

/*
 * Programa que solicita un número entero al usuario y lo escribe en pantalla.
 */
int main() {
    int numero1;
    int numero2;
    int suma;
    cout << "Escriba dos enteros separados por un espacio: ";
          cin >> numero1; cin >> numero2;
    suma = numero1 + numero2;
    cout << "Su suma es: " << suma << endl;
}
