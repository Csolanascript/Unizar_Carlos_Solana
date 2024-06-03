//-------------------------------------------------------------------------------------------
// File:   Concurrent_MultiBuffer.hpp
// Author: Carlos Solana Melero (872815) - Diego Mateo Lorente (873338)
// Date:   11-11-2023
// Coms:   Monitor que gestiona las comunicaciones entre el proceso máster y el proceso controlador
//-------------------------------------------------------------------------------------------

#ifndef CONCURRENT_MULTIBUFFER_HPP
#define CONCURRENT_MULTIBUFFER_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "MultiBuffer.hpp"

using namespace std;

template <typename T,unsigned N>
class Concurrent_MultiBuffer{
public:
    //declaracion de las operaciones
    void putTarea(const T tarea);
    void putTF(const T tarea);
    T getTarea(const unsigned pos);
    void espera();
private:
    mutex mtxConcurrent_MultiBuffer;
    MultiBuffer<T,N> mB;
    condition_variable llegaTarea[N];
    condition_variable bufferLibre;
    condition_variable zeroNews;
};


// Para poder trabajar con tipos genericos mediante templates,
// declaracion y codigo deben estar juntos
#include "Concurrent_MultiBuffer.cpp"
#endif