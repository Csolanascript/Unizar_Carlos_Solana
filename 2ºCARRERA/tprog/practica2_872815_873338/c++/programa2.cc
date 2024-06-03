/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : programa2.cc
 */

#include "programa2.h"

programa2::programa2(){
    miVector.push_back(new Read());
    miVector.push_back(new Dup());
    miVector.push_back(new Write());
    miVector.push_back(new Push(-1));    
    miVector.push_back(new Add());
    miVector.push_back(new Dup());
    miVector.push_back(new Jumpif(1));  
}