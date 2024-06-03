//-------------------------------------------------------------------------------------------
// File:   Matrix.hpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Monitor que gestiona las operaciones de escritura de la matriz y su muestre por pantalla
//-------------------------------------------------------------------------------------------

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <iomanip>  

using namespace std;

template <unsigned N>
class Matrix{
public:
//declaro mis operaciones
    Matrix();       //Constructor del monitor Matrix
    void escribe(string tarea, const string resultado, double cargaTrabajo);
    void muestra();
private:
    mutex mtxMatrix;
    int resultados[3][3];
};
// Para poder trabajar con tipos genéricos mediante templates,
// declaración y código deben estar juntos
#include "Matrix.cpp"
#endif