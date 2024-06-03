/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : write.cc
 */

#include "write.h"

void Write::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t num = pila.top();
    pila.pop();
    cout << num << endl;
    pc++;
}

void Write::muestra(){
    cout << "write";
}