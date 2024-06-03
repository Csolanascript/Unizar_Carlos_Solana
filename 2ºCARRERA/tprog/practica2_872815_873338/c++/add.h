/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : add.h
 */

#pragma once
#include "instruccion.h"

class Add: public instruccion
{
public:
    void ejecuta(stack<int32_t> &pila, int& pc) override;
    void muestra() override;
};

