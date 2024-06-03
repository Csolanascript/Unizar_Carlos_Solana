/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : programa.cc
 */

#include "programa.h"

programa::programa() {
    pc = 0;
}

programa::~programa(){
    miVector.clear();      
}

void programa::listar(){
    for (int i = 0; i < miVector.size(); i++) {
        cout << i <<"   ";
        miVector[i]->muestra();
        cout << endl;
    }
}

void programa::ejecutar(){
    stack<int32_t> pila;
    while(pc < miVector.size()) {
        miVector[pc]->ejecuta(pila, pc);     
    }
}