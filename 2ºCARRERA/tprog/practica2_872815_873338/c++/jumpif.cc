/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : jumpif.cc
 */

#include "jumpif.h"

Jumpif::Jumpif(int n)
    :l(n)
{}

void Jumpif::ejecuta(stack<int32_t> &pila, int& pc) {
    int32_t numero = pila.top();
    pila.pop();
    if(numero >= 0){
        pc = l;
    }
    else pc++;
}

void Jumpif::muestra(){
    cout << "jumpif " << l;
}