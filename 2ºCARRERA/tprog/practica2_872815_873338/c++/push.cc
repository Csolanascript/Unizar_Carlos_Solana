/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : push.cc
 */

#include "push.h"

Push::Push(int32_t c)
    :num(c)
{}
void Push::ejecuta(stack<int32_t> &pila, int& pc) {
    pila.push(num);
    pc++;
}

void Push::muestra(){
    cout << "push " << num;
}