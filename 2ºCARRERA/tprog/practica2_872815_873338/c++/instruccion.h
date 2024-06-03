/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : instruccion.h
 */

#pragma once
#include <iostream>
using namespace std;
#include <stack>

class instruccion
{
public:
    virtual void muestra() = 0;
    virtual void ejecuta(stack<int32_t> &pila, int& pc) = 0;
};

