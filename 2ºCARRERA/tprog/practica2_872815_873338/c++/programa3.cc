/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : programa3.cc
 */

#include "programa3.h"

programa3::programa3(){
    miVector.push_back(new Push(1));
    miVector.push_back(new Read());
    miVector.push_back(new Swap());  
    miVector.push_back(new Over());
    miVector.push_back(new Mul());
    miVector.push_back(new Swap());
    miVector.push_back(new Push(-1));
    miVector.push_back(new Add());
    miVector.push_back(new Dup());
    miVector.push_back(new Push(-2));
    miVector.push_back(new Add());
    miVector.push_back(new Jumpif(2));
    miVector.push_back(new Swap());
    miVector.push_back(new Write());
}