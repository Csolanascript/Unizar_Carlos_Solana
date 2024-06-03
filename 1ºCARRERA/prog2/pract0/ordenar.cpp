// Fichero de implementacion ordenar.cpp de un algoritmo de ordenacion de vectores de enteros
// de ordenación de vectores

// Definición de predicados utilizados para simplificar las especificaciones
// de las funciones genéricas de este fichero:
//
//   sonPermutacion(v1, v2, iz, de) =
//         (∀ α ϵ [iz,de].
//                (NUM β ϵ [iz,de].v1[α]=v1[β])
//              = (NUM β ϵ [iz,de].v1[α]=v2[β]) )
//
//   estaOrdenado(v, iz, de) =
//         (∀ α ϵ [iz,de-1].v[α] <= v]α+1])

#include "ordenar.hpp"

//--------------------------------------------------------------
// Pre:  a = X y b = Y
// Post: a = Y y b = X
void permutar(int &a, int &b) {
    int aux = a;
    a = b;
    b = aux;
}
//--------------------------------------------------------------
// Pre: n>0 ^ v=V_0
// Post: sonPermutacion(v,V_0,0,n-1) ^ estaOrdenado(v)
void ordenar(int v[], const int n) {
    // Ordenación de v[0,n-1] por el método de selección
    for (int i = 0; i < n - 1; ++i) {
        // Los datos de v[0..i-1] son los menores y ya están ordenados
        // Selecciona el dato menor de v[i..n-1]
        int iMenor = i;
        for (int j = i + 1; j < n; ++j) {
            // v[iMenor] es el menor de v[i..j-1]
            if (v[j] < v[iMenor]) {
                iMenor = j;
            }
            // v[iMenor] es el menor de v[i..j]
        }
        // v[iMenor] es el menor de v[i..n-1]. Permuta v[i] y v[iMenor]
        permutar(v[i], v[iMenor]);
        // Los datos de v[0..i] son los menores y ya están ordenados
    }
    // Los datos de v[0..n-1] ya están ordenados
}
//--------------------------------------------------------------
