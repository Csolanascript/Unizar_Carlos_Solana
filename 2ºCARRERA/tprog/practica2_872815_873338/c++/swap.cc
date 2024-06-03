/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : swap.cc
 */

#include "swap.h"

void Swap::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num = pila.top();
    pila.pop();
    int32_t num2 = pila.top();
    pila.pop();
    pila.push(num);
    pila.push(num2);
    pc++;
}

void Swap::muestra(){
    cout << "swap";
}