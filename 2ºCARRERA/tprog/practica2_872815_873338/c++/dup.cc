/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : dup.cc
 */

#include "dup.h"

void Dup::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num = pila.top();
    pila.push(num);
    pc++;
}

void Dup::muestra(){
    cout << "dup";
}