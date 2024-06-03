/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : mul.cc
 */

#include "mul.h"

void Mul::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num1,suma;
    suma = pila.top();
    pila.pop();
    suma *= pila.top();
    pila.pop();
    pila.push(suma);
    pc++;
}

void Mul::muestra(){
    cout << "mul";
}