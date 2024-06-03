/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : add.cc
 */

#include "add.h"

void Add::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num1,suma;
    suma = pila.top();
    pila.pop();
    suma += pila.top();
    pila.pop();
    pila.push(suma);
    pc++;
}

void Add::muestra(){
    cout << "add";
}