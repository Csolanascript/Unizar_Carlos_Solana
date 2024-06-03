/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : read.cc
 */

#include "read.h"

void Read::ejecuta(stack<int32_t> &pila, int& pc){
    int32_t entero;
    cout << "? ";
    cin >> entero;
    pila.push(entero);
    pc++;
}
void Read::muestra(){
    cout << "read";
    
}