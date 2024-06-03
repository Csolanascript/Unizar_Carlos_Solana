/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : over.cc
 */

#include "over.h"

void Over::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num = pila.top();
    pila.pop();
    int32_t num2 = pila.top();
    pila.push(num);
    pila.push(num2);
    pc++;
}

void Over::muestra(){
    cout << "over";
}