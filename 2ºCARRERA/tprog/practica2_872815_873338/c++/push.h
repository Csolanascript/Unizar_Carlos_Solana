/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : push.h
 */

#pragma once
#include "instruccion.h"

class Push: public instruccion
{
private:
    int32_t num;
public:
    Push(int32_t c);
    void ejecuta(stack<int32_t> &pila, int& pc) override;
    void muestra() override;
};