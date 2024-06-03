//*****************************************************************
// File:   MultiBuffer.hpp
// Author: PSCD-Unizar
// Date:   septiembre 2023
// Coms:   Especificación de un buffer genérico con "N" datos de tipo "T"
//         El estado de un buffer lo representaremos como 
//            (nNews,[v_{0},...,v_{N-1}],[n_{0},...,n_{N-1}])
//         donde:
//             "v_{i}" el valor en la posición "i". Si un dato está indefinido
//                  se representará como "?"
//             "n_{i}" indica si el dato iésimo es nuevo y no ha sido aún leído
//             "nNews" indicia cuántos "n_{i}" son true
//*****************************************************************

#ifndef MULTI_BUFFER
#define MULTI_BUFFER

#include <iostream>
#include <string>

using namespace std;

//1<=N
template <typename T,unsigned N>
class MultiBuffer {
public:
    //-----------------------------------------------------
    //Pre: 
    //Post: nNews=0, todos los v_i=?,todos los n_i=false
    MultiBuffer();
    //-----------------------------------------------------
    //Pre:  
    //Post: 
    //Com:  Innecesario en este caso. Se pone por completitud
    ~MultiBuffer();
    //-----------------------------------------------------
    //Pre:  0 <= pos < N
    //Post: get(pos)=v_{pos},n_{pos}=false
    //Com:  
    T get(const unsigned pos);
    //-----------------------------------------------------
    //Pre:  0 <= pos < N
    //Post: watch(pos)=v_{pos}
    //Com:  
    T watch(const unsigned pos);
    //-----------------------------------------------------
    //Pre:  0 <= pos < N
    //Post: v_{pos}=d,n_{pos}=true
    //Com:  
    void put(const T d,const unsigned pos);
    //-----------------------------------------------------
    //Pre:  
    //Post: numberNews()=nNews
    //Com:  
    unsigned numberNews();
     //-----------------------------------------------------
    //Pre:  
    //Post: size()=N
    //Com:  
    unsigned size();
    //-----------------------------------------------------
    //Pre:  0 <= pos < N
    //Post: isNew(pos)=n_{pos}
    //Com:  
    bool isNew(const unsigned pos);
    //----------------------------------------------------- 
private:
    unsigned nNews;
    T tasks[N];
    bool news[N];
};
// Para poder trabajar con tipos genéricos mediante templates,
// declaración y código deben estar juntos
#include "MultiBuffer.cpp"
#endif