/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : main.cc
 */

#include "programa1.h"
#include "programa2.h"
#include "programa3.h"

int main() {
    programa1 p1;
    programa2 p2;
    programa3 p3;
    cout << "Programa 1:" << endl;
    p1.listar();
    cout << endl << "Ejecucion:" << endl;
    p1.ejecutar();
    cout << endl << "Programa 2:" << endl;
    p2.listar();
    cout << endl << "Ejecucion:" << endl;
    p2.ejecutar();
    cout << endl << "Programa 3:" << endl;
    p3.listar();
    cout << endl << "Ejecucion:" << endl;
    p3.ejecutar();
}