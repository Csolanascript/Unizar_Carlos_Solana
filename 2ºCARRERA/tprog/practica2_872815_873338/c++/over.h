/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : over.h
 */

#pragma once
#include "instruccion.h"

class Over: public instruccion
{
private:
public:
    void ejecuta(stack<int32_t> &pila, int& pc) override;
    void muestra() override;
};