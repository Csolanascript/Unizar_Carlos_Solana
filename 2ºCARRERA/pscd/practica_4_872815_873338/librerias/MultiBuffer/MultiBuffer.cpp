//*****************************************************************
// File:   MultiBuffer.cpp
// Author: PSCD-Unizar
// Date:   Octubre 2023
// Coms:   Implemetación de la clase
//*****************************************************************

#include <cassert>

//-----------------------------------------------------
template <typename T,unsigned N>
MultiBuffer<T,N>::MultiBuffer() {

    nNews = 0 ;
    for (unsigned i=0;i<N;i++) {
        news[i] = false;
    }
}
//-----------------------------------------------------
template <typename T,unsigned N>
MultiBuffer<T,N>::~MultiBuffer() {

}
//-----------------------------------------------------
template <typename T,unsigned N>
T MultiBuffer<T,N>::get(const unsigned pos) {

    assert(0<=pos && pos<N); //innecesario, pues lo impone la pre. Pero por si acaso ....
    nNews--;
    news[pos] = false;
    return tasks[pos];
}
//-----------------------------------------------------
template <typename T,unsigned N>
T MultiBuffer<T,N>::watch(const unsigned pos) {
    assert(0<=pos && pos<N);
    return tasks[pos];
}
//-----------------------------------------------------
template <typename T,unsigned N>
void MultiBuffer<T,N>::put(const T d,const unsigned pos) {
    
    assert(0<=pos && pos<N);
    tasks[pos] = d;
    news[pos] = true;
    nNews++;
}
//-----------------------------------------------------
template <typename T,unsigned N>
unsigned MultiBuffer<T,N>::numberNews() {

    return nNews;
}
//-----------------------------------------------------
template <typename T,unsigned N>
unsigned MultiBuffer<T,N>::size() {

    return N;
}
//-----------------------------------------------------
template <typename T,unsigned N>
bool MultiBuffer<T,N>::isNew(const unsigned pos) {
        assert(0<=pos && pos<N);
    return news[pos];
}
//-----------------------------------------------------
