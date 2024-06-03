// Fichero de interfaz.hpp de un algoritmo de ordenación de vectores de enteros

// Definición de predicados utilizados para simplificar las especificaciones
// de las funciones genéricas de este fichero:
//
//   sonPermutacion(v1, v2, desde, hasta) =
//         (PT alfa EN [desde,hasta].
//                (NUM beta EN [desde,hasta].v1[alfa]=v1[beta])
//                = (NUM beta EN [desde,hasta].v1[alfa]=v2[beta]) )
//
//   estaOrdenado(v, desde, hasta) =
//         (PT alfa EN [desde,hasta-1].v[alfa] <= v]alfa+1])

#ifndef ORDENAR_HPP
#define ORDENAR_HPP

// Pre:  a = X y b = Y
// Post: a = Y y b = X
void permutar(int &a, int &b);

// Pre: 0<=n<=#v and v=v0
// Post: sonPermutacion(v, V0, 0, n-1) AND estaOrdenado(v, 0, n-1)
void ordenar(int v[], const int n);

#endif 