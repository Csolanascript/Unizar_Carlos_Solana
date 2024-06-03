/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : programa1.cc
 */

#include "programa1.h"

programa1::programa1(){
    
    miVector.push_back(new Read());
    miVector.push_back(new Read());
    miVector.push_back(new Add());
    miVector.push_back(new Write());    
}