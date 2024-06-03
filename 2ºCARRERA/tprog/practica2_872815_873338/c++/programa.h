/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : programa.h
 */

#pragma once
#include <vector>
#include "instruccion.h"
#include "add.h"
#include "dup.h"
#include "read.h"
#include "write.h"
#include "push.h"
#include "jumpif.h"
#include "mul.h"
#include "swap.h"
#include "over.h"
class programa
{
    protected:
    vector<instruccion*> miVector;
    int pc; 
    
    public:
    programa();
    ~programa();

    void listar();
    void ejecutar();

};
